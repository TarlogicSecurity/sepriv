#include "user_privileges.h"

#include <stdio.h>
#include <Windows.h>
#include <lmerr.h>
#include "lsa.h"
#include "group.h"
#include "list.h"
#include "alloc.h"
#include "str.h"
#include "utils.h"
#include "privileges.h"

static PLOCALGROUP_USERS_INFO_0 _retrieve_user_groups(LPCSTR username, OUT DWORD *groups_count) {
	PLOCALGROUP_USERS_INFO_0 groups = NULL;
	LPWSTR username_wstring = ansi_to_unicode(username);
	if (username_wstring != NULL) {
		groups = get_user_local_groups(username_wstring, groups_count);
		NULLANIZE(username_wstring);
		if (groups == NULL && GetLastError() != NERR_Success) {
			printf("[X] Error retrieving groups of %s: 0x%x\n", username, GetLastError());
		}
	}

	return groups;
}

static void _print_privileges(LPCSTR username, list_t privileges) {
	size_t i = 0;
	printf("\n");
	printf("  Privileges of %s\n", username);
	printf("===========================================\n");
	if (privileges->length == 0) {
		printf("User has no privileges or was no posible to retrieve them\n");
	}
	else {
		for (i = 0; i < privileges->length; i++) {
			printf("  %ws\n", (LPWSTR)privileges->items[i]);
		}
	}
	printf("\n");
}

static SID_NAME_USE _get_account_type(LPCSTR account_name) {
	SID_NAME_USE account_type = SidTypeGroup;
	PSID user_sid = NULL;

	user_sid = get_SID_for_name_A_Ex(account_name, NULL, &account_type);
	RELEASE(user_sid);

	return account_type;
}


int show_user_privileges(LPCSTR username) {
	int result = 1;
	SID_NAME_USE account_type = SidTypeGroup;
	PLOCALGROUP_USERS_INFO_0 groups = NULL;
	DWORD groups_count = 0;
	list_t privileges = NULL;

	account_type = _get_account_type(username);
	if (account_type == SidTypeUser) {
		groups = _retrieve_user_groups(username, &groups_count);
	}

	privileges = get_unique_privileges_of_user_and_groups(username, groups, groups_count);
	if (privileges == NULL) {
		goto end;
	}
	_print_privileges(username, privileges);

	result = 0;


end:
	if (groups) {
		free_groups(groups); groups = NULL;
	}

	if (privileges) {
		free_list(privileges); privileges = NULL;
	}

	return result;
}



static int _add_user_privilege(LPCSTR username, LPCWSTR privilege_name, PSID user_sid) {
	int status = add_lsa_privilege_in_localhost(user_sid, privilege_name);
	if (status != 0) {
		printf("[X] Error granting %ws to %s : 0x%x\n", privilege_name, username, LsaNtStatusToWinError(status));
	} else {
		printf("[+] %ws granted to %s\n", privilege_name, username);
	}
	return status;
}

static int _delete_user_privilege(LPCSTR username, LPCWSTR privilege_name, PSID user_sid) {
	int status = delete_lsa_privilege_in_localhost(user_sid, privilege_name);
	if (status != 0) {
		printf("[X] Error revoking %ws from %s : 0x%x\n", privilege_name, username, LsaNtStatusToWinError(status));
	} else {
		printf("[+] %ws revoked from %s\n", privilege_name, username);
	}
	return status;
}


int modify_user_privilege(LPCSTR username, LPCWSTR privilege_name, BOOL add_privilege) {
	PSID user_sid = NULL;
	NTSTATUS status = 0;
	int result = 1;

	user_sid = get_SID_for_name_A_Ex(username, NULL, NULL);
	if (user_sid == NULL) {
		printf("[X] Error getting user SID : 0x%x\n", GetLastError());
		goto end;
	}

	if (add_privilege) {
		status = _add_user_privilege(username, privilege_name, user_sid);
	} else {
		status = _delete_user_privilege(username, privilege_name, user_sid);
	}

	if (status == 0) {
		result = 0;
	}

end:
	RELEASE(user_sid);
	return result;
}


