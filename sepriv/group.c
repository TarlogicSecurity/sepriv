#pragma once
#include "group.h"

#pragma comment(lib, "Netapi32.lib")

PLOCALGROUP_USERS_INFO_0 get_user_local_groups(LPCWSTR username, OUT LPDWORD groups_count) {

	PLOCALGROUP_USERS_INFO_0 groups = NULL;
	DWORD total_groups_count = 0;
	NET_API_STATUS status = 0;

	status = NetUserGetLocalGroups(
		NULL, username, 0, LG_INCLUDE_INDIRECT, &(LPBYTE)groups,
		MAX_PREFERRED_LENGTH, groups_count, &total_groups_count
	);
	SetLastError(status);

	return groups;
}

void free_groups(PLOCALGROUP_USERS_INFO_0 groups) {
	NetApiBufferFree(groups);
}