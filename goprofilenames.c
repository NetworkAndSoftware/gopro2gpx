// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#include "goprofilenames.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystring.h"
#include "platform.h"
#include <ctype.h>

// see https://gopro.com/help/articles/question_answer/GoPro-Camera-File-Naming-Convention
char* make_gopro_mp4_name(int chapter, int file_number, file_name_scheme scheme)
{
  char* name = malloc(12 + 1);

  switch (scheme)
  {
  default:
  case hero_6_7_avc:
    sprintf(name, "GH%02d%04d.MP4", chapter + 1, file_number);
    break;
  case hero_6_7_hevc:
    sprintf(name, "GX%02d%04d.MP4", chapter + 1, file_number);
    break;
  case hero_fusion_front:
    if (chapter == 0)
      sprintf(name, "GPFR%04d.MP4", file_number);
    else
      sprintf(name, "GF%02d%04d.MP4", chapter, file_number);
    break;
  case hero_fusion_back:
    if (chapter == 0)
      sprintf(name, "GPBK%04d.MP4", file_number);
    else
      sprintf(name, "GB%02d%04d.MP4", chapter, file_number);
    break;
  case pre_hero_6:
    if (chapter == 0)
      sprintf(name, "GOPR%04d.MP4", file_number);
    else
      sprintf(name, "GP%2d%4d.MP4", chapter, file_number);
    break;
  }
  return name;
}

node* make_node_if_exists(int file_number, node** filenames, const char* directory, int chapter, file_name_scheme scheme)
{
  char* fullname = malloc(strlen(directory) + 1 + 12 + 1);
  char* gopro_mp4_name = make_gopro_mp4_name(chapter, file_number, scheme);
  sprintf(fullname, "%s%s%s", directory, preferred_path_separator, gopro_mp4_name);
  free(gopro_mp4_name);

  if (!file_exists(fullname))
  {
    free(fullname);
    return NULL;
  }

  return list_node(fullname);
}

// see https://gopro.com/help/articles/question_answer/GoPro-Camera-File-Naming-Convention
// ReSharper disable CppLocalVariableMightNotBeInitialized
node* get_related_filenames(const char* given_filename, int is_singlefile, int *index_of_provided_file_name)
{
  *index_of_provided_file_name = 0;
  const char* basename = strrpbrk(given_filename, path_separators);

  if (basename == NULL)
    basename = given_filename;
  else
    ++basename;

  if (is_singlefile || strlen(basename) != 12 || 0 != strcasecmp(basename + 8, ".MP4"))
    return list_node(given_filename);

  char level_1[3];
  char level_2[3];
  int file_number;

  if (3 != sscanf(basename, "%2s%2s%4d", level_1, level_2, &file_number) || file_number == 0)  // NOLINT(cert-err34-c)
    return list_node(given_filename);

  const int level_2_is_number = isdigit(level_2[0]) && isdigit(level_2[1]);
  int level_2_as_number; // meaningless if !level_2_is_number

  if (level_2_is_number)
  {
    level_2_as_number = 10 * (level_2[0] - '0') + level_2[1] - '0';

    if (level_2_as_number == 0)
      return list_node(given_filename);
  }

  // Now find adjacent chapters
  enum file_name_scheme scheme;
  int zero_based_chapter;
  
  if (!strcasecmp(level_1, "GH"))
  {
    if (!level_2_is_number)
      return list_node(given_filename);
    scheme = hero_6_7_avc;
    zero_based_chapter = level_2_as_number - 1;
  }
  else if (!strcasecmp(level_1, "GX"))
  {
    if (!level_2_is_number)
      return list_node(given_filename);
    scheme = hero_6_7_hevc;
    zero_based_chapter = level_2_as_number - 1;
  }
  else if (!strcasecmp(level_1, "GP"))
  {
    if (level_2_is_number)
    {
      zero_based_chapter = level_2_as_number;
      scheme = pre_hero_6;
    }
    else if (!strcasecmp(level_2, "FR"))
    {
      scheme = hero_fusion_front;
      zero_based_chapter = 0;
    }
    else if (!strcasecmp(level_2, "BK"))
    {
      scheme = hero_fusion_back;
      zero_based_chapter = 0;
    }
    else
      return list_node(given_filename);
  }
  else if (!strcasecmp(level_1, "GO") && !strcasecmp(level_2, "PR"))
  { // Pre-Hero6, first chapter
    zero_based_chapter = 0;
    scheme = pre_hero_6;
  }
  else if (!strcasecmp(level_1, "GF") && level_2_is_number)
  {
    scheme = hero_fusion_front;
    zero_based_chapter = level_2_as_number;
  }
  else if (!strcasecmp(level_1, "GB") && level_2_is_number)
  {
    scheme = hero_fusion_back;
    zero_based_chapter = level_2_as_number;
  }
  else
    return list_node(given_filename);

  // look for lower and higher chapters

  node* filenames = list_node(given_filename);
  int l = 1;
  char* directory = strdup(given_filename);
  strrtok(directory, path_separators);

  // TODO: look for lower and higher chapters
  for (int i = zero_based_chapter - 1; i >= 0; i--) {
    node* n = make_node_if_exists(file_number, &filenames, directory, i, scheme);
    if (!n)
      break;
    list_insert(n, &filenames);
    (*index_of_provided_file_name)++;
    l++;
  }
  for (int i = zero_based_chapter + 1; i < 100; i++) {
    node* n = make_node_if_exists(file_number, &filenames, directory, i, scheme);
    if (!n)
      break;
    list_append(n, filenames);
    l++;
  }
  free(directory);

  return filenames;
}
// ReSharper enable CppLocalVariableMightNotBeInitialized
