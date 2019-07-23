#include "gpmf.h"
#include "utils.h"
#include <stdlib.h>

size_t get_gpmf_sample_type_size(GPMF_SampleType sample_type)
{
  switch (sample_type)
  {
  case GPMF_TYPE_DOUBLE:
    return sizeof(double);
  case GPMF_TYPE_SIGNED_LONG:
    return sizeof(uint32_t);
  default:
    // TODO: add all types
    exit_not_implemented();
    return 0;  // never reached
  }
}

scaled_data scaled_data_get(GPMF_stream* ms, GPMF_SampleType sample_type)
{
  scaled_data d;
  d.repeat = GPMF_Repeat(ms);
  d.elements = GPMF_ElementsInStruct(ms);

  d.buffer_size = d.repeat * d.elements * get_gpmf_sample_type_size(sample_type);
  d.buffer = malloc(d.buffer_size);

  // Output data in LittleEnd, but no scale
  if (d.buffer != NULL)
    GPMF_ScaledData(ms,
      d.buffer,
      d.buffer_size,
      0,
      d.repeat,
      sample_type); // Output scaled data as floats
  return d;
}

void scaled_data_free(scaled_data d)
{
  if (d.buffer != NULL)
    free(d.buffer);
}

uint32_t get_GPS_Fix(GPMF_stream* ms)
{
  if (GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPSF"), GPMF_RECURSE_LEVELS))
  {
    const scaled_data data = scaled_data_get(ms, GPMF_TYPE_SIGNED_LONG);
    const uint32_t fix = *(uint32_t*)data.buffer;
    scaled_data_free(data);
    return fix;
  }
  return -1;
}

uint32_t get_GPS_Precision(GPMF_stream* ms)
{
  if (GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPSP"), GPMF_RECURSE_LEVELS))
  {
    const scaled_data data = scaled_data_get(ms, GPMF_TYPE_SIGNED_LONG);
    const uint32_t precision = *(uint32_t*)data.buffer;
    scaled_data_free(data);
    return precision;
  }
  return -1;
}

gps_location* get_gps_Locations(GPMF_stream* ms, uint32_t* repeat)
{
  if (GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPS5"), GPMF_RECURSE_LEVELS) // GoPro Hero5/6/7 GPS
    || GPMF_OK == GPMF_FindNext(ms, STR2FOURCC("GPRI"), GPMF_RECURSE_LEVELS)) // GoPro Karma GPS
  {
    const scaled_data data = scaled_data_get(ms, GPMF_TYPE_DOUBLE);

    if (data.elements != sizeof(struct gps_location) / sizeof(double))
      exit_with_error("Unexpected GPS5 or GPRI record element count found");

    *repeat = data.repeat;
    return (gps_location*)data.buffer;
  }
}
