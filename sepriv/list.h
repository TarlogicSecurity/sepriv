#pragma once

#include <stddef.h>
typedef void* list_item_t;

typedef int (*cmp_item_f)(list_item_t, list_item_t);
typedef void (*free_item_f)(list_item_t);

typedef struct {
	size_t length;
	size_t capacity;
	list_item_t* items;
	free_item_f _free_item;
	cmp_item_f _cmp_item;
} _list_t, * list_t;


list_t new_list();
void free_list(list_t list);

void set_list_cmp_item(list_t list, cmp_item_f cmp_item);
void set_list_free_item(list_t list, free_item_f free_item);



int is_item_in_list(list_t list, list_item_t item);
int append_to_list(list_t list, list_item_t item);