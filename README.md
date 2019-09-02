# sepriv

Tool to modify user privileges. It allows to add or delete privileges from an user, as well as enable or disable the privileges of processes tokens.

## A little about privileges

User privileges are not exactly the same as process privileges.

User privileges are those granted to the user in the system, which will be copied into his access token for each process. An user holds all the privileges granted to itself and those granted to the groups he belongs to, directly or transitively.

Process privileges are the privileges of the user copied into the process access token. However, some user privileges (such as SeDebugPrivilege),are restricted only to processes with high integrity (those usually executed "As Administrator"). Due to this, it is possible to see processes which belong to an user with only a subset of the privileges. 

User privileges can be granted or revoked from an user or group. Nonetheless, to use the new privileges, it is necessary to log out and start a new session.

In constrast, privileges cannot be added or removed from an process, however they can be enabled or disabled. If a privilege is disabled, the process itself can enable it, or viceversa, in every moment.

Additionally, the threads of a process can use privileges of other users by using their access tokens, this is called [impersonation](https://docs.microsoft.com/en-us/windows/win32/secauthz/client-impersonation).

## Usage
```
sepriv v0.1 - Manage privileges
Copyrigth (C) 2019 Tarlogic Security
www.tarlogic.com

sepriv.exe [/h] [/d] [/add] [/del] [/p:<pid>] [/u:<user>] <privilege_name>

Description:
  Tool to modify user privileges in processes.

Parameters:
  /h                          Prints help (this message)
  /d                          Disable privilege. (Enable by default)
  /p    pid                   PID of target process. (Parent process by default)
  /u    [domain\]username     Target username. (Current user by default)
  /l                          List all privileges with a brief description
  /lu                         List the privileges of an user. (With /u)
  /lp                         List the privileges of a process. (With /p)
  /add                        Add privilege to user. Requires restart user session. (With /u)
  /del                        Delete from privilege user. Requires restart user session. (With /u)

Examples:
  sepriv.exe SeDebugPrivilege                           => Enables SeDebugPrivilege
  sepriv.exe /d SeImpersonatePrivilege                  => Disables SeImpersonatePrivilege
  sepriv.exe /add /u:Administrator SeTcbPrivilege       => Add SeTcbPrivilege to Administrator
  sepriv.exe /del /u:Administrator SeTcbPrivilege       => Removes SeTcbPrivilege from Administrator
  sepriv.exe /l                                         => List all privileges with description
  sepriv.exe /lp /p:1337                                => List available privileges in process with pid 1337
  sepriv.exe /lu                                        => List privileges of current user
  
```

## Examples

### Listing process privileges

List privileges of current process:
```
C:\Users\user>sepriv.exe /lp

  Privileges of process 6304 [User]
===========================================
  SeShutdownPrivilege    Disabled
  SeChangeNotifyPrivilege    Enabled
  SeUndockPrivilege    Disabled
  SeIncreaseWorkingSetPrivilege    Disabled
  SeTimeZonePrivilege    Disabled
```

List privileges of a remote process:
```
C:\Users\user>tasklist | findstr "explorer.exe"
explorer.exe                  1572 Console                    4   138.012 KB

C:\Users\user>sepriv.exe /lp /p:1572

  Privileges of process 1572 [User]
===========================================
  SeShutdownPrivilege    Disabled
  SeChangeNotifyPrivilege    Enabled
  SeUndockPrivilege    Disabled
  SeIncreaseWorkingSetPrivilege    Disabled
  SeTimeZonePrivilege    Disabled

```

### Enabling SeDebugPrivilege

For this example, it is necessary to run the process "As Administrator":
```
C:\Users\user>tasklist | findstr "winlogon"
winlogon.exe                 10104 Console                    3     2.048 KB

C:\Users\user>.\sepriv.exe /lp /p:10104
[X] Error opening process[10104] : 0x5

C:\Users\user>.\sepriv.exe SeDebugPrivilege
Privilege SeDebugPrivilege enabled

C:\Users\user>.\sepriv.exe /lp /p:10104

  Privileges of process 10104 [SYSTEM]
===========================================
  SeAssignPrimaryTokenPrivilege    Disabled
  SeIncreaseQuotaPrivilege    Disabled
  SeTcbPrivilege    Enabled
  SeSecurityPrivilege    Disabled
  SeTakeOwnershipPrivilege    Disabled
  SeLoadDriverPrivilege    Disabled
  SeProfileSingleProcessPrivilege    Enabled
  SeIncreaseBasePriorityPrivilege    Enabled
  SeCreatePermanentPrivilege    Enabled
  SeBackupPrivilege    Disabled
  SeRestorePrivilege    Disabled
  SeShutdownPrivilege    Disabled
  SeDebugPrivilege    Enabled
  SeAuditPrivilege    Enabled
  SeSystemEnvironmentPrivilege    Disabled
  SeChangeNotifyPrivilege    Enabled
  SeUndockPrivilege    Disabled
  SeManageVolumePrivilege    Disabled
  SeImpersonatePrivilege    Enabled
  SeCreateGlobalPrivilege    Enabled
  SeTrustedCredManAccessPrivilege    Disabled

```

### Listing user privileges

For this functionality, it is necessary to run the process "As Administrator".

List current user privileges:
```
C:\Users\user>sepriv.exe /lu

  Privileges of USER-PC\User
===========================================
  SeSecurityPrivilege
  SeBackupPrivilege
  SeRestorePrivilege
  SeSystemtimePrivilege
  SeShutdownPrivilege
  SeRemoteShutdownPrivilege
  SeTakeOwnershipPrivilege
  SeDebugPrivilege
  SeSystemEnvironmentPrivilege
  SeSystemProfilePrivilege
  SeProfileSingleProcessPrivilege
  SeIncreaseBasePriorityPrivilege
  SeLoadDriverPrivilege
  SeCreatePagefilePrivilege
  SeIncreaseQuotaPrivilege
  SeUndockPrivilege
  SeManageVolumePrivilege
  SeImpersonatePrivilege
  SeCreateGlobalPrivilege
  SeTimeZonePrivilege
  SeCreateSymbolicLinkPrivilege
  SeChangeNotifyPrivilege
  SeDelegateSessionUserImpersonatePrivilege
  SeInteractiveLogonRight
  SeNetworkLogonRight
  SeBatchLogonRight
  SeRemoteInteractiveLogonRight
  SeIncreaseWorkingSetPrivilege
```

List privileges of a different user:
```
C:\Users\user>sepriv.exe /lu /u:another

  Privileges of USER-PC\Another
===========================================
  SeInteractiveLogonRight
  SeDenyInteractiveLogonRight
  SeDenyNetworkLogonRight
  SeDenyRemoteInteractiveLogonRight
```

## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

## Author
Eloy Pérez González [@Zer1t0](https://github.com/Zer1t0) at [@Tarlogic](https://twitter.com/Tarlogic) - https://www.tarlogic.com/en/
