// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#ifndef GPPROFILENAMES_H
#define GPPROFILENAMES_H

#include "list.h"

typedef enum file_name_scheme {
  hero_6_7_avc,
  hero_6_7_hevc,
  hero_fusion_front,
  hero_fusion_back,
  pre_hero_6
} file_name_scheme;

node* get_related_filenames(const char* given_filename, int is_singlefile, int* index_of_provided_file_name);

#endif // GPPROFILENAMES_H
