#include "str.h"
#include "alloc.h"

#include <string.h>

char* str_dup(const char* str) {
	char* str2 = MALLOC(strlen(str) + 1);
	if (str2 == NULL) {
		return NULL;
	}
	memcpy(str2, str, strlen(str) + 1);
	return str2;
}

int is_number(char* str) {
	size_t i = 0;

	if (strlen(str) == 0) {
		return 0;
	}

	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}

	return 1;
}

LPWSTR ansi_to_unicode(LPCSTR ansiiStr) {
	int nchars = 0;
	LPWSTR unicodeStr = NULL;

	nchars = MultiByteToWideChar(CP_ACP, 0, ansiiStr, -1, NULL, 0);
	if (nchars == 0) {
		return NULL;
	}
	unicodeStr = (LPWSTR)MALLOC(nchars * sizeof(WCHAR));
	if (!unicodeStr) {
		return NULL;
	}
	nchars = MultiByteToWideChar(CP_ACP, 0, ansiiStr, -1, unicodeStr, nchars);
	if (nchars == 0) {
		RELEASE(unicodeStr);
		return NULL;
	}

	return unicodeStr;
}

LPWSTR lsa_string_to_unicode(PLSA_UNICODE_STRING lsa_str) {

	LPWSTR str = NULL;
	
	TRY_CATCH(
		str = MALLOC(((SIZE_T)lsa_str->Length + 1) * sizeof(WCHAR)),
		return NULL
	);

	wcsncpy_s(str, ((SIZE_T)lsa_str->Length + 1), lsa_str->Buffer, lsa_str->Length);

	return str;
}