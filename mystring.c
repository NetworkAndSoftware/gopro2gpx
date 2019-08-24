// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#include <string.h>


// returns a pointer to the last occurance in str1 of any of the characters that are part of str2, or a null pointer if there are no matches
const char* strrpbrk(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL || *str1 == '\0' || *str2 == '\0')
    return NULL;

  for (size_t i = strlen(str1) - 1;; i--)
  {
    for (const char* p = str2; *p != '\0'; p++)
      if (*p == str1[i])
        return &(str1[i]);
    if (i == 0)
      return NULL;
  }
}

// returns the length of the last portion of str1 which consists only of characters that are part of str2
size_t strrspn(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL || *str1 == '\0' || *str2 == '\0')
    return 0;

  const size_t l1 = strlen(str1);

  for (size_t i = l1 - 1;; i--)
  {
    for (const char* p = str2; *p != str1[i]; p++)
      if (*p == '\0')
        return l1 - 1 - i;

    if (i == 0)
      return l1;
  }
}

// returns the length of the last portion of str1 which consists only of characters that are not part of str2
size_t strrcspn(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL || *str1 == '\0' || *str2 == '\0')
    return 0;

  const size_t l1 = strlen(str1);

  for (size_t i = l1 - 1;; i--)
  {
    for (const char* p = str2; *p != '\0'; p++)
      if (*p == str1[i])
        return l1 - 1 - i;

    if (i == 0)
      return l1;
  }
}

char* strrtok(char* s, const char* delim)
{
  static char* save_ptr;

  if (s == NULL)
    s = save_ptr;

  if (*s == '\0')
  {
    save_ptr = s;
    return NULL;
  }

  char* p = s + strlen(s);

  /* Scan trailing delimiters.  */
  p -= strrspn(s, delim);
  if (p == s)
  {
    save_ptr = s;
    return NULL;
  }

  *p = '\0';

  /* Find the start of the token.  */
  char* start = p - strrcspn(s, delim);
  if (start == s)
  {
    save_ptr = start;
    return s;
  }

  *(start - 1) = '\0';
  save_ptr = s;
  return start;
}