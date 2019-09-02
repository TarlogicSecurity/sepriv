#pragma once

#include <Windows.h>
#include "token_priv.h"


BOOL set_token_privilege_state(HANDLE htoken, LPCSTR privilege, BOOL enable);
PTOKEN_PRIVILEGES get_token_privileges(HANDLE hToken);
PTOKEN_USER get_token_user(HANDLE hToken);
