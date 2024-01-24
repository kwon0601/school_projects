# include <stdio.h>
# include <stdlib.h> //동적할당
# include <string.h> //strlen

typedef char* element; //element를 사용하긴 했지만, 문자열 저장을 염두하고 코드를 짜서 element타입을 다른 것으로 바꾸면 작동이 안 됩니다.
typedef struct {
	element* address; //자료가 저장될 곳의 주소
	int size; //최대 자료 개수
	int front; //맨 앞 -1(초기값=-1)
	int rear; //맨 뒤(초기값=-1)
}queue;
int x = 0; //함수의 리턴값을 받기 위한 변수
int i = 0; //반복문을 위한 변수
char strtemp[100] = "abcd";//dequeue때 문자열 반환을 위한 배열
element eletrash = NULL; //clearqueue함수에서 dequeue값을 받기 위한 변수

queue* createqueue(int size)
{
	queue* pqueue = (queue*)malloc(sizeof(queue)); //프로그램이 실행되는 도중에 생성되므로 동적할당을 합니다.
	if (pqueue == NULL)
		return NULL;
	pqueue->address = (element*)malloc(sizeof(element) * size);
	if (pqueue->address == NULL)
	{
		free(pqueue);
		return NULL;
	}
	pqueue->size = size;
	pqueue->front = -1;
	pqueue->rear = -1;
	return pqueue;
}

int isfullqueue(queue* pqueue)
{
	if (pqueue->front==(pqueue->rear+1)%(pqueue->size)) return 1;// 내용물이 꽉 찼다면 1(true)를 리턴합니다.
	return 0; // 내용물이 꽉 차지않았다면 0(false)를 리턴합니다.
}

int isemptyqueue(queue* pqueue)
{
	if (pqueue->front == pqueue->rear) return 1;// 내용물이 없다면 1(true)를 리턴합니다.
	return 0; // 내용물이 있다면 0(false)를 리턴합니다.
}

int countqueueitem(queue* pqueue)//큐안에 몇 개의 내용물이 있는지 확인합니다.
{
	return (pqueue->rear- pqueue->front+ pqueue->size)% pqueue->size; //(rear-front+size)%size:일반적으로rear-front가 개수인데,이 값이 음수일때를 위해 나머지 식들이 추가되었습니다.
}

void enqueue(queue* pqueue, element item)
{
	x = isfullqueue(pqueue);
	if (x == 1) //자료가 꽉차 있는지 확인합니다.
	{
		return ;
	}
	else
	{
		pqueue->rear = (pqueue->rear + 1) % pqueue->size;//rear에 1을 더합니다.
		pqueue->address[pqueue->rear] = (char*)malloc(1+(strlen(item))); //받은 문자열의 크기만큼 공간을 할당받아 주소를 address에 기록합니다.
		if (pqueue->address[pqueue->rear] == NULL)
			return ; //수정 NULL형 함수에서 0을 리턴하던 것을 수정했습니다.
		strcpy(pqueue->address[pqueue->rear],item); //할당받은 공간에 문자열을 적습니다.
	}
}

element dequeue(queue* pqueue)
{
	x = isemptyqueue(pqueue);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	else
	{
		pqueue->front = (pqueue->front + 1) % pqueue->size;//front에 1을 더합니다.
		strcpy(strtemp,pqueue->address[pqueue->front]); //free를 시켜줘야 하므로 문자열을 함수 반환을 위해 복사해서 저장합니다.
		free(pqueue->address[pqueue->front]);//문자열을 저장하던 공간을 free합니다.
		return strtemp;
	}
}

element queuefront(queue* pqueue)
{
	x = isemptyqueue(pqueue);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	return *(pqueue->address+ pqueue->front+1); //수정, 데이터타입이 char**과char*로 다른 것을 수정했습니다.
}

element queuerear(queue* pqueue)
{
	x = isemptyqueue(pqueue);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		return 0;
	}
	return *(pqueue->address + pqueue->rear); //수정, 데이터타입이 char**과char*로 다른 것을 수정했습니다.
}


void clearqueue(queue* pqueue)
{
	while (!isemptyqueue(pqueue)) //queue가 비워질 때까지 dequeue를 합니다.
	{
		eletrash=dequeue(pqueue);
	}
}

void destroyqueue(queue* pqueue)
{
	clearqueue(pqueue);
	free(pqueue->address);
	free(pqueue);
}

void printqueue(queue* pqueue)//queue에 있는 모든 문자열을 출력하는 함수
{
	printf("The current status of Queue : (");
	if (isemptyqueue(pqueue) == 1) //queue가 비었을때를 처리합니다.
	{
		printf(")\n\n\n\n");
		return ;
	}
	for (i = 0; i < countqueueitem(pqueue); i++)
	{
		if (i == countqueueitem(pqueue) - 1)
		{
			printf("%s%d)\n\n\n\n", *(pqueue->address + (pqueue->front + 1 + i) % pqueue->size),i+1); //마지막 문자열은 ','와 띄어쓰기를 안 넣고, //수정 출력때 숫자를 추가했습니다.
		}
		else
		{
			printf("%s%d, ", *(pqueue->address + (pqueue->front + 1 + i) % pqueue->size),i+1);//그 외의 문자열은 ','와 띄어쓰기를 안 넣습니다. //수정 출력때 숫자를 추가했습니다.
		}
	}
}