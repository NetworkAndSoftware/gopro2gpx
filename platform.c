// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#include "platform.h"

#if defined(_WINDOWS)
const char* path_separators = "\\/";
const char* preferred_path_separator = "\\";
#else
const char* path_separators = "/";
const char* preferred_path_separator = "/";
#endif
