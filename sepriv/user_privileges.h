#pragma once

#include <Windows.h>

int show_user_privileges(LPCSTR username);
int modify_user_privilege(LPCSTR username, LPCWSTR privilege_name, BOOL add_privilege);