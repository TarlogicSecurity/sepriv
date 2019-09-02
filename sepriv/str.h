#pragma once
#include "lsa.h"

char* str_dup(const char* str);
int is_number(char* str);


LPWSTR ansi_to_unicode(LPCSTR ansiiStr);
LPWSTR lsa_string_to_unicode(PLSA_UNICODE_STRING lsa_str);
