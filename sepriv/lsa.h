#pragma once
#include <Windows.h>
#include <lsalookup.h>
#include <ntsecapi.h>

NTSTATUS modify_lsa_privilege_in_localhost(PSID accountSid, LPCWSTR privilege, BOOL set);

#define add_lsa_privilege_in_localhost(account_sid, privilege) modify_lsa_privilege_in_localhost(account_sid, privilege, TRUE)
#define delete_lsa_privilege_in_localhost(account_sid, privilege) modify_lsa_privilege_in_localhost(account_sid, privilege, FALSE)


NTSTATUS enumerate_lsa_privileges_in_localhost(
	PSID accountSid,
	OUT PLSA_UNICODE_STRING* privileges, OUT PULONG privileges_count
);

NTSTATUS free_lsa_memory(PVOID ptr);