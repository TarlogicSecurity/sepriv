#include "process_privilege.h"

#include <stdio.h>
#include "alloc.h"
#include "utils.h"
#include "token.h"

int change_process_privilege(DWORD pid, LPCSTR privilege_name, BOOL enable_privilege) {
	int result = 1;
	HANDLE token_handle = NULL;
	HANDLE process_handle = NULL;

	process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (process_handle == NULL) {
		printf("[X] Error opening process[%u] : 0x%x\n", pid, GetLastError());
		goto end;
	}

	if (!OpenProcessToken(process_handle, TOKEN_ADJUST_PRIVILEGES, &token_handle)) {
		printf("[X] Error opening process token : 0x%x\n", GetLastError());
		goto end;
	}

	if (!set_token_privilege_state(token_handle, privilege_name, enable_privilege)) {
		printf("[X] Error adjusting token : 0x%x\n", GetLastError());
		goto end;
	}
	if (enable_privilege) {
		printf("[+] %s enabled\n", privilege_name);
	}
	else {
		printf("[+] %s disabled\n", privilege_name);
	}

	result = 0;
end:
	if (process_handle) {
		CloseHandle(process_handle); process_handle = NULL;
	}

	if (token_handle) {
		CloseHandle(token_handle); token_handle = NULL;
	}
	return result;
}


static LPSTR _get_token_username(HANDLE token) {
	PTOKEN_USER user = NULL;
	LPSTR username = NULL;
	LPSTR username_domain = NULL;

	TRY_CATCH(
		user = get_token_user(token),
		goto end
	);

	TRY_CATCH(
		username = get_name_A_for_SID_Ex(user->User.Sid, NULL, NULL),
		goto end
	);
	
	username_domain = get_domain_username_for_username_A(username);
	if (username_domain) {
		FREE(username);
		username = username_domain;
	}

end:
	RELEASE(user);
	return username;
}

static void _print_privileges(DWORD pid, LPCSTR username, PTOKEN_PRIVILEGES privileges) {
	DWORD i = 0;

	printf("\n");
	if (username) {
		printf("  Privileges of process %u [%s]\n", pid, username);
	}
	else {
		printf("  Privileges of process %u\n", pid);
	}
	printf("===========================================\n");
	if (privileges->PrivilegeCount == 0) {
		printf("Process has no privileges\n");
	}
	else {
		for (i = 0; i < privileges->PrivilegeCount; i++) {
			LPSTR name = get_name_by_LUID(&(privileges->Privileges[i].Luid));
			if (name == NULL) {
				continue;
			}

			if (privileges->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED) {
				printf("  %s    Enabled\n", name);
			}
			else {
				printf("  %s    Disabled\n", name);
			}

			NULLANIZE(name);
		}
	}
	printf("\n");
}

int show_process_privileges(DWORD pid) {
	int result = 1;
	HANDLE token_handle = NULL;
	HANDLE process_handle = NULL;
	PTOKEN_PRIVILEGES privileges = NULL;
	LPSTR username = NULL;

	process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (process_handle == NULL) {
		printf("[X] Error opening process[%u] : 0x%x\n", pid, GetLastError());
		goto end;
	}

	if (!OpenProcessToken(process_handle, TOKEN_QUERY, &token_handle)) {
		printf("[X] Error opening process token : 0x%x\n", GetLastError());
		goto end;
	}

	privileges = get_token_privileges(token_handle);
	if (privileges == NULL) {
		printf("[X] Error retrieving process token privileges : 0x%x\n", GetLastError());
		goto end;
	}

	username = _get_token_username(token_handle);

	_print_privileges(pid, username, privileges);

	result = 0;
end:
	RELEASE(username);
	RELEASE(privileges);
	if (process_handle) {
		CloseHandle(process_handle); process_handle = NULL;
	}

	if (token_handle) {
		CloseHandle(token_handle); token_handle = NULL;
	}
	return result;
}
