#pragma once
#include <Windows.h>
#include <LM.h>

PLOCALGROUP_USERS_INFO_0 get_user_local_groups(LPCWSTR username, OUT LPDWORD groups_count);

void free_groups(PLOCALGROUP_USERS_INFO_0 groups);