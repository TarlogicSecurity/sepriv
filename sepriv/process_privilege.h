#pragma once

#include <Windows.h>

int change_process_privilege(DWORD pid, LPCSTR privilege_name, BOOL enable_privilege);
int show_process_privileges(DWORD pid);