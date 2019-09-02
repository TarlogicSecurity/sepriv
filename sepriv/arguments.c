#include "arguments.h"

#include <stdio.h>
#include "alloc.h"
#include "token.h"
#include "str.h"
#include "utils.h"
#include "process.h"

#define MAX_ARG_SIZE 8192


static arguments_t default_arguments() {
	arguments_t arguments = NULL;
	TRY_CATCH( arguments = MALLOC(sizeof(_arguments_t)), return NULL);
	
	arguments->pid = 0;
	arguments->username = NULL;
	arguments->privilege_name_A = NULL;
	arguments->privilege_name_W = NULL;
	arguments->print_help = FALSE;
	arguments->list_privileges = FALSE;
	arguments->list_user_privileges = FALSE;
	arguments->list_process_privileges = FALSE;
	arguments->add_privilege = FALSE;
	arguments->delete_privilege = FALSE;
	arguments->enable_privilege = TRUE;

	return arguments;
}

void free_arguments(arguments_t arguments) {
	RELEASE(arguments->username);
	FREE(arguments);
}

void set_privilege_name(char* name, arguments_t arguments) {

	if (strcmp("secreatetokenprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CREATE_TOKEN_NAME_A;
		arguments->privilege_name_W = SE_CREATE_TOKEN_NAME_W;
	}
	else if (strcmp("seassignprimarytokenprivilege", name) == 0) {
		arguments->privilege_name_A = SE_ASSIGNPRIMARYTOKEN_NAME_A;
		arguments->privilege_name_W = SE_ASSIGNPRIMARYTOKEN_NAME_W;
	}
	else if (strcmp("selockmemoryprivilege", name) == 0) {
		arguments->privilege_name_A = SE_LOCK_MEMORY_NAME_A;
		arguments->privilege_name_W = SE_LOCK_MEMORY_NAME_W;
	}
	else if (strcmp("seincreasequotaprivilege", name) == 0) {
		arguments->privilege_name_A = SE_INCREASE_QUOTA_NAME_A;
		arguments->privilege_name_W = SE_INCREASE_QUOTA_NAME_W;
	}
	else if (strcmp("seunsolicitedinputprivilege", name) == 0) {
		arguments->privilege_name_A = SE_UNSOLICITED_INPUT_NAME_A;
		arguments->privilege_name_W = SE_UNSOLICITED_INPUT_NAME_W;
	}
	else if (strcmp("semachineaccountprivilege", name) == 0) {
		arguments->privilege_name_A = SE_MACHINE_ACCOUNT_NAME_A;
		arguments->privilege_name_W = SE_MACHINE_ACCOUNT_NAME_W;
	}
	else if (strcmp("setcbprivilege", name) == 0) {
		arguments->privilege_name_A = SE_TCB_NAME_A;
		arguments->privilege_name_W = SE_TCB_NAME_W;
	}
	else if (strcmp("sesecurityprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SECURITY_NAME_A;
		arguments->privilege_name_W = SE_SECURITY_NAME_W;
	}
	else if (strcmp("setakeownershipprivilege", name) == 0) {
		arguments->privilege_name_A = SE_TAKE_OWNERSHIP_NAME_A;
		arguments->privilege_name_W = SE_TAKE_OWNERSHIP_NAME_W;
	}
	else if (strcmp("seloaddriverprivilege", name) == 0) {
		arguments->privilege_name_A = SE_LOAD_DRIVER_NAME_A;
		arguments->privilege_name_W = SE_LOAD_DRIVER_NAME_W;
	}
	else if (strcmp("sesystemprofileprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SYSTEM_PROFILE_NAME_A;
		arguments->privilege_name_W = SE_SYSTEM_PROFILE_NAME_W;
	}
	else if (strcmp("sesystemtimeprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SYSTEMTIME_NAME_A;
		arguments->privilege_name_W = SE_SYSTEMTIME_NAME_W;
	}
	else if (strcmp("seprofilesingleprocessprivilege", name) == 0) {
		arguments->privilege_name_A = SE_PROF_SINGLE_PROCESS_NAME_A;
		arguments->privilege_name_W = SE_PROF_SINGLE_PROCESS_NAME_W;
	}
	else if (strcmp("seincreasebasepriorityprivilege", name) == 0) {
		arguments->privilege_name_A = SE_INC_BASE_PRIORITY_NAME_A;
		arguments->privilege_name_W = SE_INC_BASE_PRIORITY_NAME_W;
	}
	else if (strcmp("secreatepagefileprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CREATE_PAGEFILE_NAME_A;
		arguments->privilege_name_W = SE_CREATE_PAGEFILE_NAME_W;
	}
	else if (strcmp("secreatepermanentprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CREATE_PERMANENT_NAME_A;
		arguments->privilege_name_W = SE_CREATE_PERMANENT_NAME_W;
	}
	else if (strcmp("sebackupprivilege", name) == 0) {
		arguments->privilege_name_A = SE_BACKUP_NAME_A;
		arguments->privilege_name_W = SE_BACKUP_NAME_W;
	}
	else if (strcmp("serestoreprivilege", name) == 0) {
		arguments->privilege_name_A = SE_RESTORE_NAME_A;
		arguments->privilege_name_W = SE_RESTORE_NAME_W;
	}
	else if (strcmp("seshutdownprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SHUTDOWN_NAME_A;
		arguments->privilege_name_W = SE_SHUTDOWN_NAME_W;
	}
	else if (strcmp("sedebugprivilege", name) == 0) {
		arguments->privilege_name_A = SE_DEBUG_NAME_A;
		arguments->privilege_name_W = SE_DEBUG_NAME_W;
	}
	else if (strcmp("seauditprivilege", name) == 0) {
		arguments->privilege_name_A = SE_AUDIT_NAME_A;
		arguments->privilege_name_W = SE_AUDIT_NAME_W;
	}
	else if (strcmp("sesystemenvironmentprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SYSTEM_ENVIRONMENT_NAME_A;
		arguments->privilege_name_W = SE_SYSTEM_ENVIRONMENT_NAME_W;
	}
	else if (strcmp("sechangenotifyprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CHANGE_NOTIFY_NAME_A;
		arguments->privilege_name_W = SE_CHANGE_NOTIFY_NAME_W;
	}
	else if (strcmp("seremoteshutdownprivilege", name) == 0) {
		arguments->privilege_name_A = SE_REMOTE_SHUTDOWN_NAME_A;
		arguments->privilege_name_W = SE_REMOTE_SHUTDOWN_NAME_W;
	}
	else if (strcmp("seundockprivilege", name) == 0) {
		arguments->privilege_name_A = SE_UNDOCK_NAME_A;
		arguments->privilege_name_W = SE_UNDOCK_NAME_W;
	}
	else if (strcmp("sesyncagentprivilege", name) == 0) {
		arguments->privilege_name_A = SE_SYNC_AGENT_NAME_A;
		arguments->privilege_name_W = SE_SYNC_AGENT_NAME_W;
	}
	else if (strcmp("seenabledelegationprivilege", name) == 0) {
		arguments->privilege_name_A = SE_ENABLE_DELEGATION_NAME_A;
		arguments->privilege_name_W = SE_ENABLE_DELEGATION_NAME_W;
	}
	else if (strcmp("semanagevolumeprivilege", name) == 0) {
		arguments->privilege_name_A = SE_MANAGE_VOLUME_NAME_A;
		arguments->privilege_name_W = SE_MANAGE_VOLUME_NAME_W;
	}
	else if (strcmp("seimpersonateprivilege", name) == 0) {
		arguments->privilege_name_A = SE_IMPERSONATE_NAME_A;
		arguments->privilege_name_W = SE_IMPERSONATE_NAME_W;
	}
	else if (strcmp("secreateglobalprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CREATE_GLOBAL_NAME_A;
		arguments->privilege_name_W = SE_CREATE_GLOBAL_NAME_W;
	}
	else if (strcmp("setrustedcredmanaccessprivilege", name) == 0) {
		arguments->privilege_name_A = SE_TRUSTED_CREDMAN_ACCESS_NAME_A;
		arguments->privilege_name_W = SE_TRUSTED_CREDMAN_ACCESS_NAME_W;
	}
	else if (strcmp("serelabelprivilege", name) == 0) {
		arguments->privilege_name_A = SE_RELABEL_NAME_A;
		arguments->privilege_name_W = SE_RELABEL_NAME_W;
	}
	else if (strcmp("seincreaseworkingsetprivilege", name) == 0) {
		arguments->privilege_name_A = SE_INC_WORKING_SET_NAME_A;
		arguments->privilege_name_W = SE_INC_WORKING_SET_NAME_W;
	}
	else if (strcmp("setimezoneprivilege", name) == 0) {
		arguments->privilege_name_A = SE_TIME_ZONE_NAME_A;
		arguments->privilege_name_W = SE_TIME_ZONE_NAME_W;
	}
	else if (strcmp("secreatesymboliclinkprivilege", name) == 0) {
		arguments->privilege_name_A = SE_CREATE_SYMBOLIC_LINK_NAME_A;
		arguments->privilege_name_W = SE_CREATE_SYMBOLIC_LINK_NAME_W;
	}
	else if (strcmp("sedelegatesessionuserimpersonateprivilege", name) == 0) {
		arguments->privilege_name_A = SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME_A;
		arguments->privilege_name_W = SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME_W;
	}
}

arguments_t process_arguments(int argc, char** argv) {
	int i = 0;
	char argument[MAX_ARG_SIZE] = { 0 };
	char* username_aux = NULL;
	arguments_t arguments = NULL;

	TRY_CATCH(arguments = default_arguments(), return NULL);

	for (i = 1; i < argc; i++) {
		strcpy_s(argument, sizeof(argument), argv[i]);
		_strlwr_s(argument, sizeof(argument));

		if (strcmp(argument, "/h") == 0) {
			arguments->print_help = TRUE;
		}
		else if (strcmp(argument, "/l") == 0) {
			arguments->list_privileges = TRUE;
		}
		else if (strcmp(argument, "/lu") == 0) {
			arguments->list_user_privileges = TRUE;
		}
		else if (strcmp(argument, "/lp") == 0) {
			arguments->list_process_privileges = TRUE;
		}
		else if (strcmp(argument, "/add") == 0) {
			arguments->add_privilege = TRUE;
		}
		else if (strcmp(argument, "/del") == 0) {
			arguments->delete_privilege = TRUE;
		}
		else if (strcmp(argument, "/d") == 0) {
			arguments->enable_privilege = FALSE;
		}
		else if (strncmp(argument, "/p:", 3) == 0) {
			if (strlen(argument) == 3 || !is_number(argument + 3)) {
				printf("[X] Error: /p must contain a number value (PID)\n");
				goto fail;
			}

			arguments->pid = (DWORD)atoi(argument + 3);
		}
		else if (strncmp(argument, "/u:", 3) == 0) {
			if (strlen(argument) == 3) {
				printf("[X] Error: /u must contain value\n");
				goto fail;
			}

			arguments->username = (char*)str_dup(argument + 3);
			if (arguments->username == NULL) {
				printf("[X] Error: memory allocation error\n");
				goto fail;
			}
		}
		else {
			set_privilege_name(argument, arguments);
		}
	}

	if (arguments->username == NULL) {
		arguments->username = get_username_A();
		if (arguments->username == NULL) {
			printf("[X] Error: memory allocation error\n");
			goto fail;
		}
	}

	username_aux = get_domain_username_for_username_A(arguments->username);
	if (username_aux == NULL) {
		printf("[X] Error processing username: 0x%x\n", GetLastError());
		goto fail;
	}
	NULLANIZE(arguments->username);
	arguments->username = username_aux;

	if (arguments->pid == 0) {
		arguments->pid = get_ppid(GetCurrentProcessId());
	}

	return arguments;
fail:
	if (arguments) {
		free_arguments(arguments); arguments = NULL;
	}

	return NULL;
}
