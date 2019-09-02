#include "process.h"

#include <tlhelp32.h>

DWORD get_ppid(DWORD pid) {
	HANDLE process_snapshot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 process_entry = { 0 };
	DWORD ppid = 0;

	process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (process_snapshot == INVALID_HANDLE_VALUE) {
		goto end;
	}

	process_entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(process_snapshot, &process_entry)) {
		goto end;
	}

	do {
		if (process_entry.th32ProcessID == pid) {
			ppid = process_entry.th32ParentProcessID;
			break;
		}
	} while (Process32Next(process_snapshot, &process_entry));


end:
	if (process_snapshot != INVALID_HANDLE_VALUE && process_snapshot != NULL) {
		CloseHandle(process_snapshot); process_snapshot = NULL;
	}
	return ppid;
}
