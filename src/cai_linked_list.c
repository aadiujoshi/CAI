#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "cai_linked_list.h"

CAI_linked_list* CAI_create_list(size_t element_size) {
	CAI_linked_list* li = (CAI_linked_list*)malloc(sizeof(CAI_linked_list));
	if (li == NULL) {
		printf("list creation fail");
		return NULL;
	}
	li->element_size = element_size;
	li->length = 0;
	li->root = NULL;

	return li;
}

void CAI_list_add(CAI_linked_list* li, void* element) {
	CAI_list_add_at(li, element, li->length);
}

void CAI_list_add_at(CAI_linked_list* li, void* element, uint index) {
	printf("index: %i\n", index);

	if (li->root == NULL) {
		li->root = malloc(sizeof(ll_node));

		if (li->root == NULL) {
			printf("failed to init node in list at: %i\n", index);
			return;
		}

		li->root->data = malloc(li->element_size);

		if (li->root->data == NULL) {
			printf("failed to init node data in list at: %i\n", index);
			return;
		}

		memcpy(li->root->data, element, li->element_size);
		li->root->next = NULL;
		li->length++;
		return;
	}

	ll_node* prev = li->root;
	uint i = 0;
	//printf("------------------\n");
	while (i++ < index-1) {
		//printf("index: %i, cur: %p, next: %p\n", i-1, prev, prev->next);
		prev = prev->next;
	}



	//the pointer to the node at the targeted index
	ll_node* cur_tmp = prev->next;

	//node to be placed at the index
	ll_node* new_cur = malloc(sizeof(ll_node));

	if (new_cur == NULL) {
		printf("failed to init node in list at: %i\n", index);
		return;
	}

	new_cur->data = malloc(li->element_size);

	if (new_cur->data == NULL) {
		printf("failed to init node data in list at: %i\n", index);
		return;
	}

	memcpy(new_cur->data, element, li->element_size);
	new_cur->next = cur_tmp;
	prev->next = new_cur;

	li->length++;
}

void CAI_list_remove(CAI_linked_list* li, uint index) {
	if (index == 0) {
		ll_node* nxt = li->root->next;
		free(li->root->data);
		free(li->root);
		li->root = nxt;
		li->length--;
		return;
	}

	ll_node* prev = li->root;
	uint i = 0;
	while (i++ < index - 1) {
		prev = prev->next;
	}

	ll_node* target = prev->next;
	ll_node* nxt = target->next;
	free(target->data);
	free(target);

	prev->next = nxt;
	li->length--;
}

void CAI_free_list(CAI_linked_list* list){
	_CAI_free_list(list->root);
	free(list->root->data);
	free(list->root);
	free(list);
}

void _CAI_free_list(ll_node* root) {
	if (root->next == NULL) {
		return;
	}

	_CAI_free_list(root->next);

	free(root->next->data);
	free(root->next);
}

void CAI_print_list(CAI_linked_list* list) {
	ll_node* tmp = list->root;
	printf("[");
	while (tmp) {
		printf("%p, ", tmp->data);
		tmp = tmp->next;
	}
	printf("]\n");
}

void* CAI_list_get(CAI_linked_list* li, uint index) {
	ll_node* cur = li->root;
	uint i = 0;
	while (i++ < index) {
		cur = cur->next;
	}
	return cur->data;
}