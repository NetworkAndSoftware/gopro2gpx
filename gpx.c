// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "platform.h"
#include "gpx.h"

void write_gpx_header(FILE* output)
{
  fprintf(output, "\
<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n\
<gpx creator=\"gopro2gpx\" version=\"1.1\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n\
  <trk>\n\
    <name>Untitled</name>\n\
    <trkseg>\n");
}

void write_gpx_footer(FILE* output)
{
  fprintf(output, "\
    </trkseg>\n\
  </trk>\n\
</gpx>");
}

void write_gpx_track_point(FILE* output, const double latitude, const double longitude, const double elevation_wgs84, const double speed_ground, const double speed_3d, const double time)
{
  char s[256];

  time_t seconds = (time_t) floor(time);
  const double fraction = time - seconds;

  const size_t n = strftime(s, sizeof s, "%FT%T", gmtime(&seconds));
  if (n < sizeof(s))
    snprintf(s + n, (sizeof s)  - n, ".%03.0fZ", fraction * 1000);

  fprintf(output, "\
      <trkpt lat=\"%f\" lon=\"%f\">\n\
        <ele>%f</ele>\n\
        <time>%s</time>\n\
      </trkpt>\n\
",
      latitude, longitude, elevation_wgs84, s);
}