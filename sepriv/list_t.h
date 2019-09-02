#pragma once

#include <stddef.h>

typedef void* list_item_t;

typedef struct {
	size_t length;
	size_t capacity;
	list_item_t* items;
	void (*free_item)(list_item_t);
	int (*cmp_item)(list_item_t, list_item_t);
} _list_t, * list_t;


