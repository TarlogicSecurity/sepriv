#include <stdio.h>

#include <Windows.h>

#include "help.h"
#include "user_privileges.h"
#include "process_privilege.h"
#include "arguments.h"

#define TOOL_NAME "sepriv.exe"

int call_show_user_privileges(arguments_t arguments) {
	if (arguments->username == NULL) {
		printf("[X] Error: you must specify a username [/u].\n");
		return 1;
	}

	return show_user_privileges(arguments->username);
}

int call_modify_user_privilege(arguments_t arguments) {
	if (arguments->privilege_name_W == NULL) {
		printf("[X] Error: you must specify a privilege.\n");
		return 1;
	}

	if (arguments->username == NULL) {
		printf("[X] Error: you must specify a username [/u].\n");
		return 1;
	}

	return modify_user_privilege(arguments->username, arguments->privilege_name_W, arguments->add_privilege);
}

int call_change_process_privilege(arguments_t arguments) {
	if (arguments->privilege_name_A == NULL) {
		printf("[X] Error: you must specify a privilege name. Use /l to list the options.\n");
		return 1;
	}

	if (arguments->pid == 0) {
		printf("[X] Error: you must specify a process [/p].\n");
		return 1;
	}
	return change_process_privilege(arguments->pid, arguments->privilege_name_A, arguments->enable_privilege);
}

int call_show_process_privileges(arguments_t arguments) {
	if (arguments->pid == 0) {
		printf("[X] Error: you must specify a process [/p].\n");
		return 1;
	}

	return show_process_privileges(arguments->pid);
}


int call_action(arguments_t arguments) {
	if (arguments->print_help) {
		return print_help(TOOL_NAME);
	}
	else if (arguments->list_privileges) {
		return print_privilege_list();
	}
	else if (arguments->list_user_privileges) {
		return call_show_user_privileges(arguments);
	}
	else if (arguments->list_process_privileges) {
		return call_show_process_privileges(arguments);
	}
	else if (arguments->add_privilege || arguments->delete_privilege) {
		return call_modify_user_privilege(arguments);
	}
	else {
		return call_change_process_privilege(arguments);
	}
}

int main(int argc, char** argv) {
	int result = 0;
	arguments_t arguments = NULL;

	arguments = process_arguments(argc, argv);
	if (arguments == NULL) {
		return 1;
	}

	result = call_action(arguments);
	free_arguments(arguments); arguments = NULL;
	return result;
}