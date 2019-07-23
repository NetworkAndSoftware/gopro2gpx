#ifndef PLATFORM_H
#define PLATFORM_H

// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#if defined(_WIN32) || defined(_WIN64)
  #ifndef _WINDOWS
    #define WINDOWS
  #endif
  //#define snprintf _snprintf
  //#define vsnprintf _vsnprintf
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#endif

extern const char* path_separators;  // any of these can serve as a separator (either \ or / works on Windows). The first character is preferred.
extern const char* preferred_path_separator;

#endif // PLATFORM_H
