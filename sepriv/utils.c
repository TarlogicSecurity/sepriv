#include "utils.h"
#include "alloc.h"
#include "str.h"

#include <Lmcons.h>

PSID get_SID_for_name_A_Ex(LPCSTR name, OUT LPSTR* domain, OUT SID_NAME_USE* accounType) {

	PSID sid = NULL;
	DWORD cbsid = 0, domainSize = 0;
	LPSTR strSid = NULL;
	LPSTR domainname = NULL;
	BOOL ok = FALSE;
	SID_NAME_USE peUse = 0;

	ok = LookupAccountNameA(NULL, name, sid, &cbsid, domainname, &domainSize, &peUse);
	if (!ok && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		sid = (PSID)MALLOC(cbsid);
		domainname = (LPSTR)MALLOC(domainSize * sizeof(CHAR));
		if (!sid || !domainname) {
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto close_error;
		}

		ok = LookupAccountNameA(NULL, name, sid, &cbsid, domainname, &domainSize, &peUse);
	}
	if (!ok) {
		goto close_error;
	}

	if (domain) {
		*domain = domainname;
	}
	else {
		RELEASE(domainname);
	}
	if (accounType) {
		*accounType = peUse;
	}

	goto close;
close_error:
	RELEASE(domainname);
	RELEASE(sid);
close:
	return sid;
}

LPSTR get_name_A_for_SID_Ex(PSID sid, OUT LPSTR* domain, OUT SID_NAME_USE* accounType) {

	LPSTR nameStr = NULL, domainStr = NULL;
	DWORD nameLen = 0, domainLen = 0;
	BOOL ok;
	SID_NAME_USE peUse = 0;

	ok = LookupAccountSidA(NULL, sid, nameStr, &nameLen, domainStr, &domainLen, &peUse);
	if (!ok && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		nameStr = (LPSTR)MALLOC(nameLen * sizeof(TCHAR));
		domainStr = (LPSTR)MALLOC(domainLen * sizeof(TCHAR));
		if (!nameStr || !domainStr) {
			goto close_error;
		}

		ok = LookupAccountSidA(NULL, sid, nameStr, &nameLen, domainStr, &domainLen, &peUse);
	}
	if (!ok) {
		goto close_error;
	}

	if (domain) {
		*domain = domainStr;
	}
	else {
		RELEASE(domainStr);
	}
	if (accounType) {
		*accounType = peUse;
	}

	goto close;
close_error:
	RELEASE(nameStr);
	RELEASE(domainStr);
close:
	return nameStr;
}

PSID get_SID_for_name_W_Ex(LPCWSTR name, OUT LPWSTR* domain, OUT SID_NAME_USE* accounType) {
	PSID sid = NULL;
	DWORD cbsid = 0, domainSize = 0;
	LPWSTR domainname = NULL;
	BOOL ok = FALSE;
	SID_NAME_USE peUse = 0;

	ok = LookupAccountNameW(NULL, name, sid, &cbsid, domainname, &domainSize, &peUse);
	if (!ok && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		sid = (PISID)MALLOC(cbsid);
		domainname = (LPWSTR)MALLOC(domainSize * sizeof(WCHAR));
		if (!sid || !domainname) {
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto close_error;
		}

		ok = LookupAccountNameW(NULL, name, sid, &cbsid, domainname, &domainSize, &peUse);
	}
	if (!ok) {
		goto close_error;
	}

	if (domain) {
		*domain = domainname;
	}
	else {
		RELEASE(domainname);
	}
	if (accounType) {
		*accounType = peUse;
	}

	goto close;
close_error:
	RELEASE(domainname);
	RELEASE(sid);
close:
	return sid;
}

LPWSTR get_hostname_W() {
	LPWSTR hostname = NULL;
	DWORD len = 0;
	BOOL ok;

	ok = GetComputerNameExW(ComputerNameDnsHostname, hostname, &len);
	if (!ok && GetLastError() == ERROR_MORE_DATA) {
		hostname = (LPWSTR)MALLOC(((SIZE_T)len + 1) * sizeof(WCHAR));
		if (hostname == NULL) {
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return NULL;
		}
		ok = GetComputerNameExW(ComputerNameDnsHostname, hostname, &len);
	}

	if (!ok) {
		RELEASE(hostname);
	}

	return hostname;
}

LPSTR get_username_A() {
	char username[UNLEN + 1] = { 0 };
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);

	return str_dup(username);
}

LPSTR get_name_by_LUID(PLUID luid) {

	LPSTR name = NULL;
	DWORD nameLen = 0;
	BOOL ok;

	ok = LookupPrivilegeNameA(NULL, luid, name, &nameLen);

	if (!ok && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		name = (LPSTR)MALLOC(nameLen * sizeof(CHAR));
		if (name == NULL) {
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return NULL;
		}
		ok = LookupPrivilegeNameA(NULL, luid, name, &nameLen);
	}
	if (!ok) {
		RELEASE(name);
	}

	return name;
}

LPSTR get_domain_username_for_username_A(LPCSTR name) {

	LPSTR domain_username = NULL;
	DWORD domain_username_size = 0;
	LPSTR domain = NULL;
	PSID user_sid = NULL;

	if (strstr(name, "\\") != NULL) {
		return str_dup(name);
	}

	user_sid = get_SID_for_name_A_Ex(name, &domain, NULL);
	if (user_sid == NULL) {
		goto end;
	}
	NULLANIZE(user_sid);

	domain_username_size = (DWORD)(strlen(domain) + 1 + strlen(name) + 1);
	domain_username = MALLOC(domain_username_size);
	if (domain_username == NULL) {
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto end;
	}

	strcat_s(domain_username, domain_username_size, domain);
	strcat_s(domain_username, domain_username_size, "\\");
	strcat_s(domain_username, domain_username_size, name);

end:
	RELEASE(domain);

	return domain_username;
}