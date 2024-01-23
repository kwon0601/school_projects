# include <stdio.h>
# include <stdlib.h> //동적할당
typedef int element;

typedef struct linkstack{
	element content;
	struct linkstack* next; //다음 stack의 주소
}stack;

typedef struct linkstackhead { //element가 int가 아닐 수 있으므로 머리는 따로 지정합니다.
	int content;
	stack* next; //다음 stack의 주소
}stackhead;

stack* stacktemp=NULL; //임시로 stack의 주소값을 받을 변수

int i = 0; //반복문을 위한 변수
int l = 0; //clearstack을 위한 변수
element x = 0; //함수의 리턴값을 받기 위한 변수,pop에서도 쓰임


stackhead* createstack()
{
	stackhead* pstackhead = (stackhead*)malloc(sizeof(stackhead)); //프로그램이 실행되는 도중에 생성되므로 동적할당을 합니다.
	if (pstackhead == NULL)
		return NULL;
	pstackhead->content = 0;
	pstackhead->next = NULL;
	return pstackhead;
}

void push(stackhead* pstackhead, element item)
{
	stack* pstack = (stack*)malloc(sizeof(stack));
	if (pstack == NULL)
		printf("there is't memory space\n");
	else
	{
		pstack->content = item; //새로만든 스택에 content와 next를 입력합니다.
		pstack->next = pstackhead->next; //수정 새로 들어온 스택의 next가 NULL이여서 안된거였습니다.
		pstackhead->next = pstack; //머리의 next와 content를 갱신합니다.
		pstackhead->content++;
	}
}

element pop(stackhead* pstackhead)
{
	stacktemp = pstackhead->next; //현재(없애려고 하는) stack의 주소를 stacktemp에 보관합니다.
	x = pstackhead->next->content;//x에 맨위의 스택의 값을 보관합니다.
	pstackhead->next = pstackhead->next->next;//위에서 2번째 스택의 주소를 머리와 연결합니다.
	free(stacktemp); //맨 위에 스택을 없앱니다.
	pstackhead->content--;//head의 content수를 1내립니다.
	return x;// 맨위에 있었던 값을 리턴합니다.
}

element top(stackhead* pstackhead)
{
	if (pstackhead->next == NULL)
	{
		printf("your stack is empty\n"); //0이라는 정보가 들어있다고 착각할 수있기 때문에 확실히 알려줍니다.
		return 0;
	}
	return pstackhead->next->content; //맨위 스택의 정보를 줍니다.
}

int isemptystack(stackhead* pstackhead)
{
	if (pstackhead->content == 0) return 1; //empty일때 입니다.
	return 0; //내용물이 있을때 입니다.
}

int countstackitem(stackhead* pstackhead)
{
	return pstackhead->content; //현재 몇개의 자료가 있는지 리턴합니다.
}

void clearstack(stackhead* pstackhead)
{
	l = pstackhead->content;//제거하는동안 숫자가 바뀌니 변수에 미리 옮깁니다. //수정
	for (i = 0; i < l; i++) //스택의 수만큼 pop을 실행합니다.
	{
		x=pop(pstackhead);
	}
}

void destroystack(stackhead* pstackhead)
{
	clearstack(pstackhead); //스택을 비웁니다.
	free(pstackhead); //머리를 없애므로써 스택이 없어집니다.
}
