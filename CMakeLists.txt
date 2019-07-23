# CMakeList.txt : CMake project for gopro2gpx1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(gopro2gpx)
add_executable (gopro2gpx main.c GPMF_mp4reader.c gpx.c utils.c list.c platform.c mystring.c goprofilenames gpmf-parser/GPMF_parser.c)

include_directories(gopro2gpx gpmf-parser)

if (UNIX)
  set(CMAKE_BUILD_TYPE Debug)
  target_link_libraries(gopro2gpx m)
endif (UNIX)

add_definitions (-D_FILE_OFFSET_BITS=64)