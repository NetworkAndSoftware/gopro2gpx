#ifndef GPX_H
#define GPX_H

char *filename_from_filename(char *old_file_name, char *old_extension, char *new_extension);
void write_gpx_header(FILE *output);
void write_gpx_footer(FILE *output);
void write_gpx_track_point(FILE* output, const double latitude, const double longitude, const double elevation_wgs84, const double speed_ground, const double speed_3d, const double time);

#endif // GPX_H
