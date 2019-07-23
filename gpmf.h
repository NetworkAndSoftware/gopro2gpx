#ifndef GPMF_H
#define GPMF_H

#include <stdint.h>
#include <stddef.h>
#include "gpmf-parser/GPMF_parser.h"

typedef struct scaled_data {
  uint32_t repeat;
  uint32_t elements;
  uint32_t sample_size;
  uint32_t buffer_size;
  void* buffer;
} scaled_data;

size_t get_gpmf_sample_type_size(GPMF_SampleType sample_type);

scaled_data scaled_data_get(GPMF_stream* ms, GPMF_SampleType sample_type);

void scaled_data_free(scaled_data d);

uint32_t get_GPS_Fix(GPMF_stream* ms);

uint32_t get_GPS_Precision(GPMF_stream* ms);

//const double latitude, const double longitude, const double elevation_wgs84, const double speed_ground, const double speed_3d, 
typedef struct gps_location {
  double latitude;
  double longitude;
  double elevation_wgs84;
  double speed_ground;
  double speed_3d;
} gps_location;

gps_location* get_gps_Locations(GPMF_stream* ms, uint32_t* repeat);

#endif // GPMF_H
