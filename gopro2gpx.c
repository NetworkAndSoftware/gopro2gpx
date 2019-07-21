/*! @file GPMF_demo.c
 *
 *  @brief Demo to extract GPMF from an MP4
 *
 *  @version 1.0.1
 *
 *  (C) Copyright 2017 GoPro Inc (http://gopro.com/).
 *
 *  Licensed under either:
 *  - Apache License, Version 2.0, http://www.apache.org/licenses/LICENSE-2.0
 *  - MIT license, http://opensource.org/licenses/MIT
 *  at your option.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GPMF_parser.h"
#include "GPMF_mp4reader.h"
#include "gpx.h"


int main(const int argc, char* argv[])
{
  int32_t ret = GPMF_OK;
  GPMF_stream metadata_stream,* ms = &metadata_stream;
  uint32_t* payload = NULL; // buffer to store GPMF samples from the MP4.

  char* gpx_filename;

  // get file return data
  if (argc != 2 || 0 == (gpx_filename = filename_from_filename(argv[1], "mp4", "gpx"))) {
    fprintf(stderr, "usage: %s <file_with_GPMF>\n", argv[0]);
    return -1;
  }

  const size_t mp4 = OpenMP4Source(argv[1], MOV_GPMF_TRAK_TYPE, MOV_GPMF_TRAK_SUBTYPE);
  if (mp4 == 0) {
    fprintf(stderr, "error: %s is an invalid MP4/MOV\n", argv[1]);
    return -1;
  }

  const double metadata_length = GetDuration(mp4);

  if (metadata_length > 0.0) {
    
#if defined(_WIN32) || defined(_WIN64)
    FILE* gpx;
    fopen_s(&gpx, gpx_filename, "w");
#else
    FILE* gpx = fopen(gpx_filename, "w");
#endif
    if (gpx) {
      fprintf(stderr, "Failed to create GPX file '%s'.", gpx_filename);
    }

    write_gpx_header(gpx);

    const uint32_t payloads = GetNumberPayloads(mp4);

    for (uint32_t index = 0; index < payloads; index++) {
      const uint32_t payload_size = GetPayloadSize(mp4, index);
      double in = 0.0, out = 0.0; // times
      payload = GetPayload(mp4, payload, index);
      if (payload == NULL)
        goto cleanup;

      ret = GetPayloadTime(mp4, index, &in, &out);
      if (ret != GPMF_OK)
        goto cleanup;

      ret = GPMF_Init(ms, payload, payload_size);
      if (ret != GPMF_OK)
        goto cleanup;

      // Find GPS values and return scaled doubles.

      if (GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPS5"), GPMF_RECURSE_LEVELS) // GoPro Hero5/6/7 GPS
      || GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPRI"), GPMF_RECURSE_LEVELS)) // GoPro Karma GPS
      {

        const uint32_t samples = GPMF_Repeat(ms);
        const uint32_t elements = GPMF_ElementsInStruct(ms);
        const uint32_t buffer_size = samples * elements * sizeof(double);
        GPMF_stream find_stream;
        double* tmp_buffer = malloc(buffer_size);
        char units[10][6] = {""};

        if (tmp_buffer && samples && elements >= 6) {
          uint32_t i;

          // Search for any units to display
          GPMF_CopyState(ms, &find_stream);
          if (GPMF_OK == GPMF_FindPrev(&find_stream, GPMF_KEY_SI_UNITS, GPMF_CURRENT_LEVEL) || GPMF_OK == GPMF_FindPrev(&find_stream, GPMF_KEY_UNITS, GPMF_CURRENT_LEVEL)) {
            char* data = (char*)GPMF_RawData(&find_stream);
            const int s_size = GPMF_StructSize(&find_stream);
            const uint32_t unit_samples = GPMF_Repeat(&find_stream);

            for (i = 0; i < unit_samples; i++) {
              memcpy(units[i], data, s_size);
              units[i][s_size] = 0;
              data += s_size;
            }
          }

          // GPMF_FormattedData(ms, tmpbuffer, buffersize, 0, samples); //
          // Output data in LittleEnd, but no scale
          GPMF_ScaledData(ms,
            tmp_buffer,
            buffer_size,
            0,
            samples,
            GPMF_TYPE_DOUBLE); // Output scaled data as floats

          for (i = 0; i < samples; i++) {

            const double sample_time = in + (double)i / (double)samples * (out - in);

            write_gpx_track_point(gpx, tmp_buffer[5 * i + 0], tmp_buffer[5 * i + 1], tmp_buffer[5 * i + 2], tmp_buffer[5 * i + 3], tmp_buffer[5 * i + 4], sample_time);
          }
          free(tmp_buffer);
        }
      }

      GPMF_ResetState(ms);
    }

  cleanup:
    if (payload)
      FreePayload(payload);
    CloseSource(mp4);

    write_gpx_footer(gpx);
    fclose(gpx);
  }

  return ret;
}
