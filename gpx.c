#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gpx.h"

#if defined(_WIN32) || defined(_WIN64)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

char* filename_from_filename(char* old_file_name, char* old_extension, char* new_extension)
{
  char* extension = strrchr(old_file_name, '.');
  if (extension == NULL)
    return NULL;

  if (0 != strcasecmp(++extension, old_extension))
    return NULL;

  const size_t base_length = extension - old_file_name;
  const size_t new_filename_size = base_length + strlen(new_extension) + 1;
  char* new_file_name = malloc(new_filename_size);
  if (new_file_name != NULL) {
#if defined(_WIN32) || defined(_WIN64)
    strncpy_s(new_file_name, new_filename_size, old_file_name, base_length);
    strcpy_s(new_file_name + base_length, new_filename_size - base_length, new_extension);
#else
    strncpy(new_file_name, old_file_name, base_length);
    strcpy(new_file_name + base_length, new_extension);
#endif
  }
  return new_file_name;
}

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

  const size_t n = strftime(s, sizeof(s), "%FT%T", gmtime(&seconds));
  if (n < sizeof(s))
    snprintf(s + n, sizeof(s) - n, ".%03.0fZ", fraction * 1000);

  fprintf(output, "\
      <trkpt lat=\"%f\" lon=\"%f\">\n\
        <ele>%f</ele>\n\
        <time>%s</time>\n\
      </trkpt>\n\
",
      latitude, longitude, elevation_wgs84, s);
}