#include "List_Array.h"
#pragma warning (disable:4996)

#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

int select_number = 0;

void print_list(list* plist);

int main()
{
	list* list_one = create_list(int_compare,200);
	while (select_number != 3)
	{
		printf("In (0), Search (1), Out (2), Exit (3) : ");
		scanf("%d", &select_number);

		switch (select_number)
		{
		case 0: //In
		{
			int* insert_number = (int*)malloc(sizeof(int));
			printf("In : ");
			scanf("%d", insert_number);
			insert_list(list_one, (void*)insert_number);
			print_list(list_one);
			break;
		}
		case 1: //Search
		{
			printf("Search : ");
			int search_number = 0;
			scanf("%d", &search_number);
			if (retrieve(list_one, (void*)&search_number)) printf("My List has %d.\n", search_number);
			else printf("My List does not have %d.\n", search_number);
			print_list(list_one);
			break;
		}
		case 2: //Out
		{
			printf("Out : ");
			int remove_number = 0;
			scanf("%d", &remove_number);
			if (remove_content(list_one, (void*)&remove_number)) printf("%d was removed.\n", remove_number);
			else printf("My List does not have %d.\n", remove_number);
			print_list(list_one);
			break;
		}
		case 3: break; //Exit
		default: printf("Please write number 0,1,2 or3\n");
		}
		printf("\n");
	}
	destroy_list(list_one);

	return 0;
}

void print_list(list* plist)
{
	element Data = NULL;

	printf("The current status of List : ");
	if (is_empty_list(plist))
	{
		printf("There is not numbers\n");
		return;
	}

	for (i = 0; i<list_count(plist); i++)
	{
		printf("%d", *(int*)*(plist->content + i));
		if (i != list_count(plist)-1) printf(", "); //마지막 숫자라면 ", "가 출력 안 됩니다. 
	}
	printf("\n");


}