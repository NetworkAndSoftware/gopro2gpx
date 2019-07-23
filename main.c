/*! @file main.c
 *
 *  @brief extract GPMF from an GoPro MP4 into .GPX file
 *
 *  @version 1.0.1
 *
 *  Based on gpmf-parser example, provided by GoPro
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
#include <ctype.h>
#include <stdarg.h>

#include "gpmf-parser/GPMF_parser.h"
#include "GPMF_mp4reader.h"
#include "platform.h"
#include "gpx.h"
#include "utils.h"
#include "mystring.h"
#include "list.h"
#include "goprofilenames.h"


typedef struct mp4_with_name {
  const char* file_name;
  size_t mp4;
} mp4_with_name;

node* open_mp4_with_name(char* file_name)
{
  const size_t mp4 = OpenMP4Source(file_name, MOV_GPMF_TRAK_TYPE, MOV_GPMF_TRAK_SUBTYPE);
  if (mp4 == 0)
    return NULL;

  const double metadata_length = GetDuration(mp4);
  if (metadata_length <= 0.0)
    return NULL;

  mp4_with_name* m = malloc(sizeof(mp4_with_name));
  if (m == NULL)
    exit_with_error("Out of memory");

  m->file_name = file_name;
  m->mp4 = mp4;

  return list_node(m);
}

// whittle down the list of files starting at the given name
node* filter_and_open_mp4s(node* file_names, char* provided_name, int index_of_provided_file_name)
{
  if (file_names == NULL || provided_name == NULL)
    return NULL;

  node* mp4s = open_mp4_with_name(strdup(provided_name));

  if (mp4s == NULL)
    return NULL; // forget it if we can't open the file with the provided name

  for (int i = index_of_provided_file_name - 1; i >= 0; i--)
  { // inefficient to iterate this way, but doesn't matter.
    char* file_name = (char*)list_element_at_index(i, file_names)->value;

    node* mp4 = open_mp4_with_name(file_name);

    if (mp4 == NULL)
      break;  // break the chain

    list_insert(mp4, &mp4s);
  }

  for (node* n = list_element_at_index(index_of_provided_file_name, file_names)->next; n != NULL; n = n->next)
  {
    node* mp4 = open_mp4_with_name((char*)n->value);

    if (mp4 == NULL)
      break; // break the chain
    list_append(mp4, mp4s);
  }

  return mp4s;
}

int main(const int argc, char* argv[])
{
  const char* usage = "usage: %s [-s] <GH010123.MP4>\n\n\tWhere GH010123.MP4 is the name of a GoPro video with GPS metadata.\n\n\tUse -s to process a single file only.\nVersion: 0.0.1\n";

  const int is_singlefile = (argc == 3 && 0 == strcmp(argv[1], "-s"));

  if (argc != 2 && !is_singlefile)
    exit_with_error(usage, file_name(argv[0]));

  char* provided_file_name = is_singlefile ? argv[2] : argv[1];

  if (!file_exists(provided_file_name))
    exit_with_error("File '%s' does not exist.", provided_file_name);

  node* mp4s;
  {
    int index_of_provided_file_name;
    node* input_files = get_related_filenames(provided_file_name, is_singlefile, &index_of_provided_file_name);

    if (input_files == NULL)
      exit_with_error(usage, file_name(argv[0]));

    mp4s = filter_and_open_mp4s(input_files, provided_file_name, index_of_provided_file_name);

    list_free(&input_files);
  }

  if (mp4s == NULL)
    exit_with_error("No valid input files found.");

  char* gpx_filename = filename_from_filename(((mp4_with_name*)mp4s->value)->file_name, "mp4", "gpx");

  if (gpx_filename == NULL)
    exit_with_error(usage, file_name(argv[0]));

  puts("Input files:");
  for (node* n = mp4s; n != NULL; n = n->next)
    printf(" %s", ((mp4_with_name*)n->value)->file_name);
  printf("\nOutput file: %s\n", gpx_filename);

  FILE* gpx = fopen(gpx_filename, "w");

  if (gpx == NULL)
    exit_with_error("Failed to create GPX file '%s'.", gpx_filename);

  write_gpx_header(gpx);
  uint32_t* payload = NULL; // buffer to store GPMF samples from the MP4.

  for (;;)
  {
    mp4_with_name* _mp4_with_name = (mp4_with_name*)list_popleft(&mp4s);
    if (_mp4_with_name == NULL)
      break;

    const uint32_t payloads = GetNumberPayloads(_mp4_with_name->mp4);

    for (uint32_t index = 0; index < payloads; index++) {
      const uint32_t payload_size = GetPayloadSize(_mp4_with_name->mp4, index);

      payload = GetPayload(_mp4_with_name->mp4, payload, index);
      if (payload != NULL)
      {
        double in = 0.0, out = 0.0; // times

        if (GPMF_OK == GetPayloadTime(_mp4_with_name->mp4, index, &in, &out))
        {
          GPMF_stream metadata_stream, * ms = &metadata_stream;

          if (GPMF_OK == GPMF_Init(ms, payload, payload_size))
          {
            // Find GPS values and return scaled doubles.

            if (GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPS5"), GPMF_RECURSE_LEVELS) // GoPro Hero5/6/7 GPS
              || GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPRI"), GPMF_RECURSE_LEVELS)) // GoPro Karma GPS
            {
              const uint32_t samples = GPMF_Repeat(ms);
              const uint32_t elements = GPMF_ElementsInStruct(ms);
              const uint32_t buffer_size = samples * elements * sizeof(double);
              double* tmp_buffer = malloc(buffer_size);

              if (tmp_buffer && samples && elements >= 5) {
                // Output data in LittleEnd, but no scale
                GPMF_ScaledData(ms,
                  tmp_buffer,
                  buffer_size,
                  0,
                  samples,
                  GPMF_TYPE_DOUBLE); // Output scaled data as floats

                for (uint32_t i = 0; i < samples; i++) {

                  const double sample_time = in + (double)i / (double)samples * (out - in);

                  write_gpx_track_point(gpx, tmp_buffer[5 * i + 0], tmp_buffer[5 * i + 1], tmp_buffer[5 * i + 2], tmp_buffer[5 * i + 3], tmp_buffer[5 * i + 4], sample_time);
                }
                free(tmp_buffer);
              }
            }
            GPMF_ResetState(ms);
          }
        }
      }
    }

    CloseSource(_mp4_with_name->mp4);
    free((void*)_mp4_with_name->file_name);
    free(_mp4_with_name);
  }

  FreePayload(payload);
  write_gpx_footer(gpx);
  return 0;
}
