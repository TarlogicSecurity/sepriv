#include "token.h"
#include "alloc.h"

#pragma comment(lib, "Advapi32.lib")

// TOKEN_ADJUST_PRIVILEGES required
BOOL set_token_privilege_state(HANDLE htoken, LPCSTR privilege_name, BOOL enable) {
	LUID luid = { 0 };
	BOOL ok = FALSE;
	TOKEN_PRIVILEGES tokenPrivs = { 0 };

	ok = LookupPrivilegeValueA(NULL, privilege_name, &luid);
	if (!ok) {
		return FALSE;
	}

	// set properties to enable privilege
	tokenPrivs.PrivilegeCount = 1;
	tokenPrivs.Privileges[0].Luid = luid;
	if (enable) {
		tokenPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else {
		tokenPrivs.Privileges[0].Attributes = 0;
	}

	ok = AdjustTokenPrivileges(htoken, FALSE, &tokenPrivs, 0, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL);
	if (!ok || GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		return FALSE;
	}

	return TRUE;
}

LPVOID get_token_attr_info(HANDLE hToken, TOKEN_INFORMATION_CLASS tokenInformationClass) {
	BOOL ok = FALSE;
	DWORD size = 0;
	LPVOID token_info_addr = NULL;

	// first call to get the size
	ok = GetTokenInformation(hToken, tokenInformationClass, token_info_addr, size, &size);

	if (!ok && (GetLastError() == ERROR_INSUFFICIENT_BUFFER || GetLastError() == ERROR_BAD_LENGTH)) {
		token_info_addr = (LPVOID)MALLOC(size);
		if (token_info_addr == NULL) {
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return NULL;
		}
		ok = GetTokenInformation(hToken, tokenInformationClass, token_info_addr, size, &size);
	}

	if (!ok) {
		RELEASE(token_info_addr);
	}

	return token_info_addr;
}

PTOKEN_PRIVILEGES get_token_privileges(HANDLE hToken) {
	return get_token_attr_info(hToken, TokenPrivileges);
}

PTOKEN_USER get_token_user(HANDLE hToken) {
	return get_token_attr_info(hToken, TokenUser);
}