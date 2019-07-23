// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "platform.h"
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include "mystring.h"

char* filename_from_filename(const char* old_file_name, char* old_extension, char* new_extension)
{
  char* extension = strrchr(old_file_name, '.');
  if (extension == NULL)
    return NULL;
  
  if (0 != strcasecmp(++extension, old_extension))
    return NULL;

  const size_t base_length = strlen(old_file_name) - strlen(extension) - 1;
  const size_t new_filename_length = base_length + 1 + strlen(new_extension);
  char* new_file_name = malloc(new_filename_length + 1);
  if (new_file_name != NULL) {
    strncpy(new_file_name, old_file_name, base_length + 1);
    strcpy(new_file_name + base_length + 1, new_extension);
  }
  return new_file_name;
}

int file_exists(const char *filename)
{

#if defined(_WINDOWS)
  struct __stat64 buffer;
  const int s = _stat64(filename, &buffer);
#else
  struct stat buffer;
  const int s = stat(filename, &buffer);
#endif
    
  return (s == 0);
}

const char *file_name(const char *path)
{
  if (path == NULL)
    return NULL;
  
  const char* p = strrpbrk(path, path_separators);
  return p == NULL ? path : ++p;
}

void exit_with_error(const char* format, ...)
{
  va_list argptr;
  va_start(argptr, format);
  vfprintf(stderr, format, argptr);
  va_end(argptr);
  fprintf(stderr, "\n");
  exit(-1);
}

void exit_not_implemented()
{
  exit_with_error("Work in progress. This functionality has not been implemented yet!\n");
}

void update_average(const double d, double* average, int count)
{
  *average = (*average * (double) count + d) / ((double) count + 1);
}
