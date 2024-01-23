//헤더파일을 2개 쓰다보니 Stack_Array.h에서 아래와 같이 수정했습니다.
//element > elementtwo 
//스택의 이름stack > stackchar
//함수들은 뒤에 char를 모두 붙였습니다.
//이미 선언된 변수(i,l,x),헤더파일들은 재선언하지 않았습니다.
typedef char elementtwo;//스택의 자료가 int 에서 char로 바뀌었습니다.
//그 이외에는 동일합니다.

typedef struct {
	elementtwo* stack;
	int size;
	int top;
}stackchar;

stackchar* createstackchar(int size)
{
	stackchar* pstack = (stackchar*)malloc(sizeof(stackchar)); //프로그램이 실행되는 도중에 생성되므로 동적할당을 합니다.
	if (pstack == NULL)
		return NULL;
	pstack->stack = (elementtwo*)malloc(sizeof(elementtwo) * size); //elementtwo 배열을 size만큼 담을 수 있는 공간을 받아 그 주소를 stack에 기록합니다.
	if (pstack->stack == NULL)
	{
		free(pstack);
		return NULL;
	}
	pstack->size = size;
	pstack->top = -1;
	return pstack;
}

void destroystackchar(stackchar* pstack)
{
	free(pstack->stack); //size는 어차피 양수이므로 강의 영상에서의 조건문이 필요없다고 생각해서 안넣었습니다.
	free(pstack);
}

int isfullstackchar(stackchar* pstack)
{
	if (pstack->size > (pstack->top) + 1) return 0;// 공간이 남아있다면 0(false)를 리턴합니다.
	return 1; // 공간이 없다면 1(true)를 리턴합니다.
}

int isemptystackchar(stackchar* pstack)
{
	if (pstack->top == -1) return 1;// 내용물이 없다면 1(true)를 리턴합니다.
	return 0; // 내용물이 있다면 0(false)를 리턴합니다.
}

int countstackitemchar(stackchar* pstack)//스택안에 몇 개의 내용물이 있는지 확인합니다.
{
	return (pstack->top) + 1;
}

void pushchar(stackchar* pstack, elementtwo item)
{
	x = isfullstackchar(pstack);
	if (x == 1) //자료가 꽉차 있는지 확인합니다.
	{
		printf("your stack is full,so you can't push\n");
	}
	else
	{
		pstack->stack[++pstack->top] = item;
	}
}

elementtwo popchar(stackchar* pstack)
{
	x = isemptystackchar(pstack);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		printf("your stack is empty,so you can't pop\n");
		return 0;
	}
	else
	{
		return pstack->stack[pstack->top--];///현재의 top을 리턴하고,그 밑의 값이 top이 됩니다.
	}
}

elementtwo topchar(stackchar* pstack)
{
	x = isemptystackchar(pstack);
	if (x == 1) //자료가 비었는지 확인합니다.
	{
		printf("your stack is empty\n"); //0이라는 정보가 들어있다고 착각할 수있기 때문에 확실히 알려줍니다.
		return 0;
	}
	return pstack->stack[pstack->top];//stack 제일 위에 정보를 줍니다.
}

void clearstackchar(stackchar* pstack) 
{
	l = pstack->top; //제거하는동안 숫자가 바뀌니 변수에 미리 옮깁니다.
	for (i = 0; i <= l; i++) //pstack의 top+1만큼 반복합니다.
	{
		x = popchar(pstack); //pstack의 자료를 제거합니다.
	}
	pstack->top = -1;
}