#ifndef GPX_H
#define GPX_H

// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

void write_gpx_header(FILE *output);
void write_gpx_footer(FILE *output);
void write_gpx_track_point(FILE* output, const double latitude, const double longitude, const double elevation_wgs84, const double speed_ground, const double speed_3d, const double time);

#endif // GPX_H
