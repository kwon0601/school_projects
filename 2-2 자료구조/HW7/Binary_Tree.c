#pragma warning(disable:4996)

#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _string_h
#define _string_h
#include <string.h>
#endif
#include"Binary_Tree.h"

#ifndef _repeat_variable
#define _repeat_variable
int i = 0; //반복문을 위한 변수
int l = 0;
#endif

int main()
{
	char input_string[50];
	printf("Input a string : ");
	scanf("%s", input_string);
	printf("\n");

	tree_head* tree_one = bt_create();
	l = strlen(input_string);
	for (i = 0; i < l; i++)
	{
		bt_insert(tree_one, (void*)(&input_string[i]));
	}
	printf("Pre-order : ");
	bt_traverse(tree_one->root, print_char, PRE_ORDER);
	printf("\n\nIn-order : ");
	bt_traverse(tree_one->root, print_char, INFIX_ORDER);
	printf("\n\nPost-order : ");
	bt_traverse(tree_one->root, print_char, POST_ORDER);
	printf("\n\nBreath First : ");
	bt_traverse(tree_one->root, print_char, BREATH_FIRST);
	printf("\n");
	bt_destroy(tree_one);
	return 0;
}
