// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#ifndef MYSTRING_H
#define MYSTRING_H

// returns a pointer to the last occurance in str1 of any of the characters that are part of str2, or a null pointer if there are no matches
const char* strrpbrk(const char* str1, const char* str2);

// returns the length of the last portion of str1 which consists only of characters that are part of str2
size_t strrspn(const char* str1, const char* str2);

char* strrtok(char* s, const char* delim);

#endif // MYSTRING_H
