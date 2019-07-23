// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#ifndef UTILS_H
#define UTILS_H

char* filename_from_filename(const char* old_file_name, char* old_extension, char* new_extension);
int file_exists(const char* filename);
const char* file_name(const char* path);

void exit_not_implemented();
void exit_with_error(const char* format, ...);

void update_average(const double d, double* average, int count);

#endif // UTILS_H
