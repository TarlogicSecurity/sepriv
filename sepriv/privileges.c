#include "privileges.h"

#include <Windows.h>
#include "lsa.h"
#include "alloc.h"
#include "utils.h"
#include "str.h"

int privilege_cmp(list_item_t item1, list_item_t item2) {
	return wcscmp((LPWSTR)item1, (LPWSTR)item2) == 0;
}

void privilege_free(list_item_t item) {
	FREE(item);
}

list_t get_unique_privileges_of_user_and_groups(const char* username, PLOCALGROUP_USERS_INFO_0 groups, DWORD groups_count) {

	PSID user_sid = NULL, group_sid = NULL;
	SID_NAME_USE account_type = SidTypeGroup;
	list_t list = NULL;
	PLSA_UNICODE_STRING privileges = NULL;
	ULONG privileges_count = 0, i = 0;
	DWORD j = 0;
	NTSTATUS status = 0;
	LPWSTR privilege_name_W = NULL;

	TRY_CATCH(list = new_list(), goto end);
	set_list_cmp_item(list, privilege_cmp);
	set_list_free_item(list, privilege_free);

	TRY_CATCH(
		user_sid = get_SID_for_name_A_Ex(username, NULL, &account_type),
		goto fail
	);

	status = enumerate_lsa_privileges_in_localhost(user_sid, &privileges, &privileges_count);
	if (status == 0) {
		for (i = 0; i < privileges_count; i++) {
			privilege_name_W = lsa_string_to_unicode(privileges + i);
			if (privilege_name_W == NULL) {
				continue;
			}
			if (!is_item_in_list(list, privilege_name_W)) {
				append_to_list(list, (list_item_t)privilege_name_W);
			}
			else {
				NULLANIZE(privilege_name_W);
			}
		}

		free_lsa_memory(privileges); privileges = NULL;
	}
	NULLANIZE(user_sid);

	for (j = 0; j < groups_count; j++) {
		TRY_CATCH(
			group_sid = get_SID_for_name_W_Ex(groups[j].lgrui0_name, NULL, NULL),
			continue
		);

		status = enumerate_lsa_privileges_in_localhost(group_sid, &privileges, &privileges_count);
		if (status == 0) {
			for (i = 0; i < privileges_count; i++) {
				privilege_name_W = lsa_string_to_unicode(privileges + i);
				if (privilege_name_W == NULL) {
					continue;
				}
				if (!is_item_in_list(list, privilege_name_W)) {
					append_to_list(list, (list_item_t)privilege_name_W);
				}
				else {
					NULLANIZE(privilege_name_W);
				}
			}
			free_lsa_memory(privileges); privileges = NULL;
		}

		NULLANIZE(group_sid);
	}

	goto end;
fail:
	if (list) {
		free_list(list); list = 0;
	}
end:

	return list;
}