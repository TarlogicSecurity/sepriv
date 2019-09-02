#include "list.h"
#include "alloc.h"


#define INITIAL_CAPACITY 16

void default_free_list_item(list_item_t item) {
}

int default_cmp_list_item(list_item_t list_item, list_item_t external_item) {
	return list_item == external_item;
}


list_t new_list() {
	list_t list = NULL;
	
	TRY_CATCH(list = MALLOC(sizeof(_list_t)), 
		return NULL);

	list->length = 0;
	list->capacity = INITIAL_CAPACITY;
	list->_free_item = default_free_list_item;
	list->_cmp_item = default_cmp_list_item;

	TRY_CATCH(list->items = MALLOC(list->capacity * sizeof(void*)), 
		NULLANIZE(list));

	return list;
}

void free_list(list_t list) {
	size_t i = 0;

	for (i = 0; i < list->length; i++) {
		list->_free_item(list->items[i]);
		list->items[i] = NULL;
	}
	list->length = 0;
	list->capacity = 0;
	NULLANIZE(list->items);
	FREE(list);
}


void set_list_cmp_item(list_t list, cmp_item_f cmp_item) {
	list->_cmp_item = cmp_item;
}

void set_list_free_item(list_t list, free_item_f free_item) {
	list->_free_item = free_item;
}

int is_item_in_list(list_t list, list_item_t item) {
	size_t i = 0;
	for (i = 0; i < list->length; i++) {
		if (list->_cmp_item(list->items[i], item)) {
			return 1;
		}
	}

	return 0;
}


static int increment_list_capacity(list_t list) {
	list_item_t* aux_ptr = NULL;
	size_t new_capacity = list->capacity * 2;

	TRY_CATCH(aux_ptr = REALLOC(list->items, new_capacity * sizeof(list_item_t)),
		return -1);

	list->items = aux_ptr;
	list->capacity = new_capacity;
	return 0;
}

int append_to_list(list_t list, list_item_t item) {
	if (list->length == list->capacity) {
		if (-1 == increment_list_capacity(list)) {
			return -1;
		}
	}

	list->items[list->length] = item;
	list->length++;

	return 0;
}
