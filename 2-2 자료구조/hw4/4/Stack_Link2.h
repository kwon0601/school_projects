//헤더파일을 2개 쓰다보니 Stack_Link.h에서 아래와 같이 수정했습니다.
//element > elementtwo 
//스택의 이름stack > stackchar, linkstack>linkstackchar
//헤드 스택의 이름 stackhead>stackheadchar, linkstackhead>linkstackheadchar
// 변수 이름 stacktemp>stacktempchar
// 변수 x대신 p를 설정했습니다.
//함수들은 뒤에 char를 모두 붙였습니다.
//이미 선언된 변수(i,l),헤더파일들은 재선언하지 않았습니다.
typedef char elementtwo;//스택의 자료가 int 에서 char로 바뀌었습니다.
//그 이외에는 동일합니다.

typedef struct linkstackchar {
	elementtwo content;
	struct linkstackchar* next; //다음 stackchar의 주소
}stackchar;

typedef struct linkstackheadchar { //elementtwo가 int가 아닐 수 있으므로 머리는 따로 지정합니다.
	int content;
	stackchar* next; //다음 stackchar의 주소
}stackheadchar;

stackchar* stacktempchar = NULL; //임시로 stack의 주소값을 받을 변수

elementtwo p = 0; //함수의 리턴값을 받기 위한 변수,pop에서도 쓰임

stackheadchar* createstackchar()
{
	stackheadchar* pstackhead = (stackheadchar*)malloc(sizeof(stackheadchar)); //프로그램이 실행되는 도중에 생성되므로 동적할당을 합니다.
	if (pstackhead == NULL)
		return NULL;
	pstackhead->content = 0;
	pstackhead->next = NULL;
	return pstackhead;
}

void pushchar(stackheadchar* pstackhead, elementtwo item)
{
	stackchar* pstack = (stackchar*)malloc(sizeof(stackchar));
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

elementtwo popchar(stackheadchar* pstackhead)
{
	stacktempchar = pstackhead->next; //현재(없애려고 하는) stackchar의 주소를 stacktempchar에 보관합니다.
	x = pstackhead->next->content;//x에 맨위의 스택의 값을 보관합니다.
	pstackhead->next = pstackhead->next->next;//위에서 2번째 스택의 주소를 머리와 연결합니다.
	free(stacktempchar); //맨 위에 스택을 없앱니다.
	pstackhead->content--;//head의 content수를 1내립니다.
	return x;// 맨위에 있었던 값을 리턴합니다.
}

elementtwo topchar(stackheadchar* pstackhead)
{
	if (pstackhead->next == NULL)
	{
		printf("your stack is empty\n"); //0이라는 정보가 들어있다고 착각할 수있기 때문에 확실히 알려줍니다.
		return 0;
	}
	return pstackhead->next->content; //맨위 스택의 정보를 줍니다.
}

int isemptystackchar(stackheadchar* pstackhead)
{
	if (pstackhead->content == 0) return 1; //empty일때 입니다.
	return 0; //내용물이 있을때 입니다.
}

int countstackitemchar(stackheadchar* pstackhead)
{
	return pstackhead->content; //현재 몇개의 자료가 있는지 리턴합니다.
}

void clearstackchar(stackheadchar* pstackhead)
{
	l = pstackhead->content;//제거하는동안 숫자가 바뀌니 변수에 미리 옮깁니다. //수정
	for (i = 0; i < l; i++) //스택의 수만큼 pop을 실행합니다.
	{
		x = popchar(pstackhead);
	}
}

void destroystackchar(stackheadchar* pstackhead)
{
	clearstackchar(pstackhead); //스택을 비웁니다.
	free(pstackhead); //머리를 없애므로써 스택이 없어집니다.
}
