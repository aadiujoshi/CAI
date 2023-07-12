
#include <stdio.h>
#include "cai_linked_list.h"

int main() {
	


	return 0;
}

/* LINKED_LIST TESTING (WORKS 100%)

	printf("hi\n");

	linked_list* list = create_list(sizeof(int));

	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
	int i4 = 4;
	int i5 = 5;

	list_add(list, &i1);
	list_add(list, &i2);
	list_add(list, &i3);
	list_add(list, &i4);
	list_add(list, &i5);

	printf("%i\n", *((int*)list_get(list, 0)));
	printf("%i\n", *((int*)list_get(list, 1)));
	printf("%i\n", *((int*)list_get(list, 2)));
	printf("%i\n", *((int*)list_get(list, 3)));
	printf("%i\n", *((int*)list_get(list, 4)));

	print_list(list);
	printf("list size: %i", list->length);

	list_remove(list, 0);

	print_list(list);
	printf("list size: %i", list->length);


	list_remove(list, 3);

	print_list(list);
	printf("list size: %i", list->length);

	list_remove(list, 1);

	print_list(list);
	printf("list size: %i", list->length);


	free_list(list);

	printf("YAYYYYYYY");

*/