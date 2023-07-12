#pragma once

#include <memory.h>
#include "cai_fwd_dec.h"

#define uint unsigned int

typedef struct _ll_node ll_node;
typedef struct _ll_node {
	ll_node* next;
	void* data;
} ll_node;

typedef struct _CAI_linked_list {
	ll_node* root;
	size_t length;
	size_t element_size;
} CAI_linked_list;


CAI_linked_list* CAI_create_list(size_t element_size);
void CAI_list_add(CAI_linked_list* list, void* element);
void CAI_list_add_at(CAI_linked_list* list, void* element, uint index);
void* CAI_list_get(CAI_linked_list* list, uint index);
void CAI_list_remove(CAI_linked_list* list, uint index);

// If elements are pointers, they are assumed to be already freed
void CAI_free_list(CAI_linked_list* list);
void _CAI_free_list(ll_node* root);

void CAI_print_list(CAI_linked_list* list);