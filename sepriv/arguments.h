#pragma once

#include <Windows.h>

typedef struct {
	DWORD pid;
	LPSTR username;
	LPSTR privilege_name_A;
	LPWSTR privilege_name_W;
	BOOL print_help;
	BOOL list_privileges;
	BOOL list_user_privileges;
	BOOL list_process_privileges;
	BOOL add_privilege;
	BOOL delete_privilege;
	BOOL enable_privilege;
} _arguments_t, * arguments_t;

arguments_t process_arguments(int argc, char** argv);

void free_arguments(arguments_t arguments);
