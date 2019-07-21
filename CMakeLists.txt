# CMakeList.txt : CMake project for gopro2gpx1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(gopro2gpx)
include_directories(gpmf-parser)
add_executable (gopro2gpx gopro2gpx.c GPMF_mp4reader.c gpx.c gpmf-parser/GPMF_parser.c)

#if (UNIX)
  target_link_libraries(gopro2gpx m)
#endif

# TODO: Add tests and install targets if needed.
