#include "lsa.h"
#include <ntstatus.h>

#include "alloc.h"
#include "utils.h"

void init_unicode_string(LSA_UNICODE_STRING* unicode_string, LPCWSTR string) {
	unicode_string->Buffer = (PWSTR)string;
	unicode_string->Length = (USHORT)wcslen(unicode_string->Buffer) * sizeof(WCHAR);
	unicode_string->MaximumLength = unicode_string->Length + sizeof(WCHAR);
}


NTSTATUS modify_lsa_privilege(PSID account_sid, PLSA_UNICODE_STRING lsa_hostname, LPCWSTR privilege, BOOL set) {

	LSA_OBJECT_ATTRIBUTES object_attrs = { 0 };
	LSA_UNICODE_STRING user_rigths = { 0 };
	LSA_HANDLE hpolicy = NULL;
	NTSTATUS status = 0;

	status = LsaOpenPolicy(lsa_hostname, &object_attrs, POLICY_LOOKUP_NAMES | POLICY_CREATE_ACCOUNT, &hpolicy);
	if (status != STATUS_SUCCESS) {
		goto close;
	}

	init_unicode_string(&user_rigths, privilege);
	if(set) {
		status = LsaAddAccountRights(hpolicy, account_sid, &user_rigths, 1);
	}
	else {
		status = LsaRemoveAccountRights(hpolicy, account_sid, FALSE, &user_rigths, 1);
	}

close:

	if (hpolicy) {
		LsaClose(hpolicy); hpolicy = NULL;
	}

	return status;
}

NTSTATUS modify_lsa_privilege_in_localhost(PSID accountSid, LPCWSTR privilege, BOOL set) {
	LSA_UNICODE_STRING lsa_hostname = { 0 };
	NTSTATUS status = 0;
	LPWSTR hostname = get_hostname_W();
	
	if (hostname == NULL) {
		return STATUS_NO_MEMORY;
	}

	init_unicode_string(&lsa_hostname, hostname);
	status = modify_lsa_privilege(accountSid, &lsa_hostname, privilege, set);

	RELEASE(hostname);
	return status;
}


NTSTATUS enumerate_lsa_privileges(
	PSID account_sid, PLSA_UNICODE_STRING lsa_hostname, 
	OUT PLSA_UNICODE_STRING *privileges, OUT PULONG privileges_count
) {
	LSA_OBJECT_ATTRIBUTES object_attrs = { 0 };
	LSA_HANDLE hpolicy = NULL;
	NTSTATUS status = 0;

	status = LsaOpenPolicy(lsa_hostname, &object_attrs, POLICY_LOOKUP_NAMES, &hpolicy);
	if (status != STATUS_SUCCESS) {
		goto close;
	}

	status = LsaEnumerateAccountRights(hpolicy, account_sid, privileges, privileges_count);

close:

	if (hpolicy) {
		LsaClose(hpolicy); hpolicy = NULL;
	}

	return status;
}


NTSTATUS enumerate_lsa_privileges_in_localhost(
	PSID account_sid,
	OUT PLSA_UNICODE_STRING* privileges, OUT PULONG privileges_count
) {
	LSA_UNICODE_STRING lsa_hostname = { 0 };
	NTSTATUS status = 0;
	LPWSTR hostname = get_hostname_W();

	if (hostname == NULL) {
		return STATUS_NO_MEMORY;
	}

	init_unicode_string(&lsa_hostname, hostname);
	status = enumerate_lsa_privileges(account_sid, &lsa_hostname, privileges, privileges_count);

	RELEASE(hostname);
	return status;
}

NTSTATUS free_lsa_memory(PVOID ptr) {
	return LsaFreeMemory(ptr);
}