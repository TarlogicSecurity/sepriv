#pragma once
#include <Windows.h>

PSID get_SID_for_name_A_Ex(LPCSTR name, OUT LPSTR* domain, OUT SID_NAME_USE* accounType);
PSID get_SID_for_name_W_Ex(LPCWSTR name, OUT LPWSTR* domain, OUT SID_NAME_USE* accounType);
LPSTR get_name_A_for_SID_Ex(PSID sid, OUT LPSTR* domain, OUT SID_NAME_USE* accounType);
LPSTR get_domain_username_for_username_A(LPCSTR name);
LPWSTR get_hostname_W();
LPSTR get_username_A();
LPSTR get_name_by_LUID(PLUID luid);