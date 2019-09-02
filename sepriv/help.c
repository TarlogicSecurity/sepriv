#include "help.h"

#include <stdio.h>

void print_usage(LPCSTR tool_name) {
	printf("%s [/h] [/d] [/add] [/del] [/p:<pid>] [/u:<user>] <privilege_name>\n", tool_name);
}

static void print_banner() {
	printf("sepriv v0.1 - Manage privileges\n");
	printf("Copyrigth (C) 2019 Tarlogic Security\n");
	printf("www.tarlogic.com\n");
}

int print_help(LPCSTR tool_name) {
	printf("\n");
	print_banner();
	printf("\n");
	print_usage(tool_name);
	printf("\n");
	printf("Description:\n");
	printf("  Tool to modify user privileges in processes.\n");
	printf("\n");
	printf("Parameters:\n");
	printf("  /h                          Prints help (this message)\n");
	printf("  /d                          Disable privilege. (Enable by default)\n");
	printf("  /p    pid                   PID of target process. (Parent process by default)\n");
	printf("  /u    [domain\\]username     Target username. (Current user by default)\n");
	printf("  /l                          List all privileges with a brief description\n");
	printf("  /lu                         List the privileges of an user. (With /u)\n");
	printf("  /lp                         List the privileges of a process. (With /p)\n");
	printf("  /add                        Add privilege to user. Requires restart user session. (With /u)\n");
	printf("  /del                        Delete from privilege user. Requires restart user session. (With /u)\n");
	printf("\n");

	printf("Examples:\n");
	printf("  %s SeDebugPrivilege                           => Enables SeDebugPrivilege\n", tool_name);
	printf("  %s /d SeImpersonatePrivilege                  => Disables SeImpersonatePrivilege\n", tool_name);
	printf("  %s /add /u:Administrator SeTcbPrivilege       => Add SeTcbPrivilege to Administrator\n", tool_name);
	printf("  %s /del /u:Administrator SeTcbPrivilege       => Removes SeTcbPrivilege from Administrator\n", tool_name);
	printf("  %s /l                                         => List all privileges with description\n", tool_name);
	printf("  %s /lp /p:1337                                => List available privileges in process with pid 1337\n", tool_name);
	printf("  %s /lu                                        => List privileges of current user\n", tool_name);
	printf("\n");

	return 0;
}

int print_privilege_list() {
	printf("\n");
	printf("   Name                                        Description\n");
	printf("=============================================================================================================\n");
	printf("  SeAssignPrimaryTokenPrivilege                Replace a process-level token\n");
	printf("  SeAuditPrivilege                             Generate security audits\n");
	printf("  SeBackupPrivilege                            Back up files and directories\n");
	printf("  SeChangeNotifyPrivilege                      Bypass traverse checking\n");
	printf("  SeCreateGlobalPrivilege                      Create global objects\n");
	printf("  SeCreatePagefilePrivilege                    Create a pagefile\n");
	printf("  SeCreatePermanentPrivilege                   Create permanent shared objects\n");
	printf("  SeCreateSymbolicLinkPrivilege                Create symbolic links\n");
	printf("  SeCreateTokenPrivilege                       Create a token object\n");
	printf("  SeDebugPrivilege                             Debug programs\n");
	printf("  SeDelegateSessionUserImpersonatePrivilege    Impersonate a client after authentication\n");
	printf("  SeEnableDelegationPrivilege                  Enable computer and user accounts to be trusted for delegation\n");
	printf("  SeImpersonatePrivilege                       Impersonate a client after authentication\n");
	printf("  SeIncreaseBasePriorityPrivilege              Increase scheduling priority\n");
	printf("  SeIncreaseQuotaPrivilege                     Adjust memory quotas for a process\n");
	printf("  SeIncreaseWorkingSetPrivilege                Increase a process working set\n");
	printf("  SeLoadDriverPrivilege                        Load and unload device drivers\n");
	printf("  SeLockMemoryPrivilege                        Lock pages in memory\n");
	printf("  SeMachineAccountPrivilege                    Add workstations to domain\n");
	printf("  SeManageVolumePrivilege                      Manage the files on a volume\n");
	printf("  SeProfileSingleProcessPrivilege              Profile single process\n");
	printf("  SeRelabelPrivilege                           Modify an object label\n");
	printf("  SeRemoteShutdownPrivilege                    Force shutdown from a remote system\n");
	printf("  SeRestorePrivilege                           Restore files and directories\n");
	printf("  SeSecurityPrivilege                          Manage auditing and security log\n");
	printf("  SeShutdownPrivilege                          Shut down the system\n");
	printf("  SeSyncAgentPrivilege                         Synchronize directory service data\n");
	printf("  SeSystemEnvironmentPrivilege                 Modify firmware environment values\n");
	printf("  SeSystemProfilePrivilege                     Profile system performance\n");
	printf("  SeSystemtimePrivilege                        Change the system time\n");
	printf("  SeTakeOwnershipPrivilege                     Take ownership of files or other objects\n");
	printf("  SeTcbPrivilege                               Act as part of the operating system\n");
	printf("  SeTimeZonePrivilege                          Change the time zone\n");
	printf("  SeTrustedCredManAccessPrivilege              Access Credential Manager as a trusted caller\n");
	printf("  SeUndockPrivilege                            Remove computer from docking station\n");
	printf("  SeUnsolicitedInputPrivilege                  Not applicable\n");
	printf("\n");

	return 0;
}
