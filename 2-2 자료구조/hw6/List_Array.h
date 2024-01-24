//중복으로 선언은 피합니다
#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _stdlib_h
#define _stdlib_h
#include <stdlib.h>
#endif

#ifndef _string_h
#define _string_h
#include <string.h>
#endif

typedef void* element;

int i = 0;//반복문을 위한 변수
int j = 0;//반복문을 위한 변수
int l = 0;//destroy_list함수에서 노드 개수 기억을 위해 사용

typedef struct List {
	int count;
	element* content;
	int pos;
	int (*compare)(element compare_one, element compare_two);
}list;

int int_compare(element compare_one, element compare_two) //앞의 것-뒤의 것을 리턴합니다.
{
	return (*((int*)compare_one)) - (*((int*)compare_two));
}

list* create_list(int (*compare_sort)(element compare_one, element compare_two),int size)
{
	list* plist = (list*)malloc(sizeof(list));
	if (plist == NULL)
		return 0;
	plist->count = 0;
	plist->content = (element*)malloc(sizeof(element) * size);
	plist->pos = 0;
	plist->compare = compare_sort;
	return plist;
}

int search(int* ploc, list* plist, element target)
{
#define COMPARE plist->compare(target,*(plist->content+(*ploc)))
	*ploc = 0;
	if (plist->count == 0) return 0; //배열이 비었을때

	for (; (*ploc)<plist->count&& COMPARE >= 0; (*ploc)++)
	{
		if (COMPARE == 0) return 1;
	}
	return 0;
}

void move_array(list* plist, int ploc,int direction)//list안 array를 특정한 방향으로 미는 함수 (ploc번째 항부터 적용)
{
	if (direction==1) //오른쪽으로 밈
	{
		for (i = 0; i < (plist->count)-ploc; i++)
		{
			*(plist->content + plist->count-i)= *(plist->content + plist->count-1-i);
		}
	}
	else //왼쪽으로 밈
	{
		for (i = 0; i < (plist->count) - ploc-1; i++)
		{
			*(plist->content+ploc + i) = *(plist->content +ploc+ i+1);
		}
	}
}

void insert_list(list* plist, element insert_content)
{
	int ploc = 0;
	int search_result = search(&ploc, plist, insert_content);
	if (search_result) return; //항목이 이미 있으면 아무 것도 안합니다.

	if (ploc == plist->count)//맨 끝에 붙는 경우
	{
		*(plist->content + ploc) = insert_content; 
	}
	else //그 외의 경우
	{
		move_array(plist, ploc, 1);
		*(plist->content + ploc) = insert_content;
	}
	(plist->count)++; //이 줄은 공통이라서 따로 뺏습니다.
}

int retrieve(list* plist, element search_content)
{
	int ploc = 0;
	int search_result = search(&ploc, plist, search_content);
	return search_result;
}

int remove_content(list* plist, element eliminate_content) 
{
	int ploc = 0;
	int search_result = search(&ploc, plist, eliminate_content);

	if (search_result)
	{
		free((int*)(*(plist->content + ploc)));  //배열 안 내용을 저장하던 공간
		move_array(plist, ploc, -1);
		(plist->count)--;
		return 1;
	}
	else return 0;
}

int is_empty_list(list* plist)
{
	if (plist->count == 0) return 1;
	else return 0;
}

int list_count(list* plist)
{
	return plist->count;
}

void destroy_list(list* plist)
{
	l = list_count(plist);
	element content_temp = NULL;
	for (i = 0; i < l; i++) //노드 개수만큼 remove_content를 실행 합니다.
	{
		content_temp = *(plist->content);
		remove_content(plist, content_temp);
	}
	free(plist);
}

//traverse는 array특성상 만들 필요가 없다고 생각해서 안 만들었습니다.