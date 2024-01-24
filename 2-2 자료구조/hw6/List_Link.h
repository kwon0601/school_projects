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

typedef struct Listnode {
	element content;
	struct Listnode* next;
}listnode; //편의상 list_node대신 listnode로 했습니다.
 
typedef struct List {
	int count;
	listnode* front;
	listnode* rear;
	listnode* pos;
	int (*compare)(element compare_one,element compare_two);
}list;

int int_compare(element compare_one, element compare_two) //앞의 것-뒤의 것을 리턴합니다.
{
	return (*((int*)compare_one)) - (*((int*)compare_two)); 
}

list* create_list(int (*compare_sort)(element compare_one, element compare_two))
{
	list* plist = (list*)malloc(sizeof(list));
	if (plist ==NULL) 
		return 0;
	plist->count = 0;
	plist->front = NULL;
	plist->rear = NULL;
	plist->pos = NULL;
	plist->compare = compare_sort;
	return plist;
}

int search(listnode** ppre,listnode** ploc,list* plist,element target)
{
#define COMPARE plist->compare(target,(*ploc)->content)
	*ppre = NULL;
	*ploc = plist->front;

	if (*ploc == NULL) return 0; //노드가 없을때

	while (*ploc != NULL&&COMPARE>=0)
	{
		if(COMPARE == 0) return 1;
		*ppre = *ploc;
		*ploc = (*ploc)->next;
	}
	return 0;
}

void insert_list(list* plist, element insert_content)
{
	listnode* ploc=NULL;
	listnode* ppre=NULL;
	int search_result = search(&ppre, &ploc, plist, insert_content);

	if (search_result) return; //항목이 이미 있으면 아무 것도 안합니다.

	listnode* plistnode = (listnode*)malloc(sizeof(listnode));
	if (plistnode == NULL)
		return;

	plistnode->content = insert_content; //이 두줄은 공통이라서 따로 뺏습니다.
	(plist->count)++;

	if (ppre == NULL) //plist에 바로 붙는 경우
	{
		plistnode->next = plist->front;
		plist->front = plistnode;
	}
	else
	{
		if (ppre == plist->rear) //맨 끝에 붙는 경우
		{
			plistnode->next = NULL;
			ppre->next = plistnode;
			plist->rear = plistnode;
		}
		else//그 외
		{
			plistnode->next = ppre->next;
			ppre->next = plistnode;
		}
	}
}

int retrieve(list* plist, element search_content)
{
	listnode* ploc = NULL;
	listnode* ppre = NULL;
	int search_result = search(&ppre, &ploc, plist, search_content);
	return search_result;
}

int remove_node(list* plist, element eliminate_content)
{
	listnode* ploc = NULL;
	listnode* ppre = NULL;
	int search_result = search(&ppre, &ploc, plist, eliminate_content);

	if (search_result)
	{
		if (ppre == NULL) plist->front = ploc->next; //front가 삭제
		else if (ppre == plist->rear)//rear가 삭제
		{
			ppre->next = ploc->next;
			plist->front = ppre;
		}
		else ppre->next = ploc->next;

		free((int*)(ploc->content));  //노드 안 내용을 저장하던 공간
		free(ploc); //노드 자체
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
	element content_temp=NULL;
	for (i = 0; i < l; i++) //노드 개수만큼 remove_node를 실행 합니다.
	{
		content_temp = plist->front->content;
		remove_node(plist, content_temp);
	}
	free(plist);
}

int traverse_list(list* plist, int from_where, element* pdata_out)
{
	if (from_where == 0 || plist->pos == NULL) plist->pos = plist->front;
	else plist->pos = plist->pos->next;
	
	if (plist->pos == NULL) return 0;
	*pdata_out = plist->pos->content; 
	return 1;
}