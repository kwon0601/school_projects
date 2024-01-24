# include <stdio.h>
# include <stdlib.h> //동적할당
# include <string.h> //strlen

typedef char* element; //element를 사용하긴 했지만, 문자열 저장을 염두하고 코드를 짜서 element타입을 다른 것으로 바꾸면 작동이 안 됩니다.
typedef struct Queue {
	element content; //자료
	struct Queue* next; //다음 queue의 주소
}queue;

typedef struct {
	queue* front;//맨 앞의 주소
	int count; //자료의 개수
	queue* rear; //맨 뒤의 주소
}queuehead;

int x = 0; //함수의 리턴값을 받기 위한 변수 
int i = 0; //반복문을 위한 변수
char strtemp[100] = "abcd";//dequeue때 문자열 반환을 위한 배열 
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
	queuetemp->content = (char*)malloc(1 + (strlen(item))); //받은 문자열의 크기만큼 공간을 할당받아 그 주소를 address에 기록합니다.
	if (queuetemp->content == NULL)
	{
		free(queuetemp);
		return;
	}
	strcpy(queuetemp->content, item); //할당받은 공간에 문자열을 적습니다.
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
		strcpy(strtemp, queuetemp->content); //free를 시켜줘야 하므로 문자열을 함수 반환을 위해 복사해서 저장합니다.
		free(queuetemp->content); //front에서 문자열을 저장하던 공간을 free합니다.
		free(queuetemp); //front를 free합니다.
		pqueuehead->count--; //count를 내립니다.
		return strtemp;
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

void printqueue(queuehead* pqueuehead)//queue에 있는 모든 문자열을 출력하는 함수
{
	printf("The current status of Queue : (");
	if (isemptyqueue(pqueuehead) == 1) //queue가 비었을때를 처리합니다.
	{
		printf(")\n\n\n\n");
		return ;
	}
	//맨처음에 queuetemp에 front의 주소를 담고, 문자열이 출력될때마다, queuetemp를 다음 queue를 가리키게 해서 출력을 합니다.
	for (i = 0, queuetemp = pqueuehead->front; i < countqueueitem(pqueuehead); i++) 
	{
		if (i == countqueueitem(pqueuehead) - 1)
		{
			printf("%s%d)\n\n\n\n", queuetemp->content,i+1); //마지막 문자열은 ','와 띄어쓰기를 안 넣고,//수정 출력때 숫자를 추가했습니다.
		}
		else
		{
			printf("%s%d, ", queuetemp->content,i+1);//그 외의 문자열은 ','와 띄어쓰기를 안 넣습니다.//수정 출력때 숫자를 추가했습니다.
		}
		queuetemp = queuetemp->next;
	}
}