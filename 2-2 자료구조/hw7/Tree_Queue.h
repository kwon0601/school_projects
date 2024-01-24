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

#ifndef _element
#define _element
typedef void* element;
#endif

typedef struct Queue {
	element content; //자료
	struct Queue* next; //다음 queue의 주소
}queue;

typedef struct {
	queue* front;//맨 앞의 주소
	int count; //자료의 개수
	queue* rear; //맨 뒤의 주소
}queuehead;

#ifndef _repeat_variable
#define _repeat_variable
int i = 0; //반복문을 위한 변수
int l = 0;
#endif

int x = 0; //함수의 리턴값을 받기 위한 변수 
//char strtemp[100] = "abcd";//dequeue때 문자열 반환을 위한 배열 
element eletrash = NULL; //clearqueue함수에서 dequeue값을 받기 위한 변수  
queue* queuetemp = NULL;//queue주소를 임시로 저장하는 변수

queuehead* createqueue()
{
	queuehead* pqueuehead = (queuehead*)malloc(sizeof(queuehead)); //프로그램이 실행되는 도중에 생성되므로 동적할당을 합니다.
	if (pqueuehead == NULL)
		return NULL;
	pqueuehead->count = 0;
	pqueuehead->front = NULL;
	pqueuehead->rear = NULL;
	return pqueuehead;
}

int isemptyqueue(queuehead* pqueuehead)
{
	if (pqueuehead->count == 0) return 1;// 내용물이 없다면 1(true)를 리턴합니다.
	return 0; // 내용물이 있다면 0(false)를 리턴합니다.
}

int countqueueitem(queuehead* pqueuehead)//큐안에 몇 개의 내용물이 있는지 확인합니다.
{
	return pqueuehead->count;
}

void enqueue(queuehead* pqueuehead, element item)
{
	queuetemp = (queue*)malloc(sizeof(queue)); //새로운 queue를 넣을 공간을 만듭니다.
	if (queuetemp == NULL)
		return;
	queuetemp->content = item;
	queuetemp->next = NULL;
	if (((pqueuehead->count)++) == 0) //기존에 queue가 없을 경우
	{
		pqueuehead->front = queuetemp; //이번에 생긴 queue가 front가 됩니다.
	}
	else //기존에 queue가 있을 경우
	{
		pqueuehead->rear->next = queuetemp; //기존 queue의 rear에 새로만든 queue를 이어 줍니다.
	}
	pqueuehead->rear = queuetemp; //새로운 queue가 rear가 되었으니 이를 갱신합니다.
}

element dequeue(queuehead* pqueuehead)
{
	x = isemptyqueue(pqueuehead);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	else
	{
		queuetemp = pqueuehead->front;// front주소를 queuetemp에 임시 저장합니다.
		pqueuehead->front = pqueuehead->front->next; //2번째에 있던 queue가 queue front가 됩니다.
		void* return_address = queuetemp->content; //간직하던 content를 임시로 저장합니다.
		free(queuetemp); //front를 free합니다.
		pqueuehead->count--; //count를 내립니다.
		return return_address;
	}
}

element queuefront(queuehead* pqueuehead)
{
	x = isemptyqueue(pqueuehead);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	return pqueuehead->front->content;
}

element queuerear(queuehead* pqueuehead)
{
	x = isemptyqueue(pqueuehead);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	return pqueuehead->rear->content;
}

void clearqueue(queuehead* pqueuehead)
{
	while (!isemptyqueue(pqueuehead)) //queue가 비워질 때까지 dequeue를 합니다.
	{
		eletrash = dequeue(pqueuehead);
	}
}

void destroyqueue(queuehead* pqueuehead)
{
	clearqueue(pqueuehead);
	free(pqueuehead);
}
