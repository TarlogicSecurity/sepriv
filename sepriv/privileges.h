#pragma once

#include "list.h"
#include "group.h"

list_t get_unique_privileges_of_user_and_groups(const char* username, PLOCALGROUP_USERS_INFO_0 groups, DWORD groups_count);