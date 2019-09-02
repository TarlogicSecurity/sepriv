#pragma once
#include <Windows.h>

#define MALLOC(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define REALLOC(ptr, size) HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptr, size)
#define FREE(ptr) HeapFree(GetProcessHeap(), 0, ptr)
#define NULLANIZE(ptr) FREE(ptr); ptr = NULL
#define RELEASE(ptr) if(ptr){NULLANIZE(ptr);}


#define TRY_CATCH(expr, action)	if(!(expr)) { action; }
