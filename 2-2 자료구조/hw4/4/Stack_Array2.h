//��������� 2�� ���ٺ��� Stack_Array.h���� �Ʒ��� ���� �����߽��ϴ�.
//element > elementtwo 
//������ �̸�stack > stackchar
//�Լ����� �ڿ� char�� ��� �ٿ����ϴ�.
//�̹� ����� ����(i,l,x),������ϵ��� �缱������ �ʾҽ��ϴ�.
typedef char elementtwo;//������ �ڷᰡ int ���� char�� �ٲ�����ϴ�.
//�� �̿ܿ��� �����մϴ�.

typedef struct {
	elementtwo* stack;
	int size;
	int top;
}stackchar;

stackchar* createstackchar(int size)
{
	stackchar* pstack = (stackchar*)malloc(sizeof(stackchar)); //���α׷��� ����Ǵ� ���߿� �����ǹǷ� �����Ҵ��� �մϴ�.
	if (pstack == NULL)
		return NULL;
	pstack->stack = (elementtwo*)malloc(sizeof(elementtwo) * size); //elementtwo �迭�� size��ŭ ���� �� �ִ� ������ �޾� �� �ּҸ� stack�� ����մϴ�.
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
	free(pstack->stack); //size�� ������ ����̹Ƿ� ���� ���󿡼��� ���ǹ��� �ʿ���ٰ� �����ؼ� �ȳ־����ϴ�.
	free(pstack);
}

int isfullstackchar(stackchar* pstack)
{
	if (pstack->size > (pstack->top) + 1) return 0;// ������ �����ִٸ� 0(false)�� �����մϴ�.
	return 1; // ������ ���ٸ� 1(true)�� �����մϴ�.
}

int isemptystackchar(stackchar* pstack)
{
	if (pstack->top == -1) return 1;// ���빰�� ���ٸ� 1(true)�� �����մϴ�.
	return 0; // ���빰�� �ִٸ� 0(false)�� �����մϴ�.
}

int countstackitemchar(stackchar* pstack)//���þȿ� �� ���� ���빰�� �ִ��� Ȯ���մϴ�.
{
	return (pstack->top) + 1;
}

void pushchar(stackchar* pstack, elementtwo item)
{
	x = isfullstackchar(pstack);
	if (x == 1) //�ڷᰡ ���� �ִ��� Ȯ���մϴ�.
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
	if (x == 1) //�ڷᰡ ������� Ȯ���մϴ�.
	{
		printf("your stack is empty,so you can't pop\n");
		return 0;
	}
	else
	{
		return pstack->stack[pstack->top--];///������ top�� �����ϰ�,�� ���� ���� top�� �˴ϴ�.
	}
}

elementtwo topchar(stackchar* pstack)
{
	x = isemptystackchar(pstack);
	if (x == 1) //�ڷᰡ ������� Ȯ���մϴ�.
	{
		printf("your stack is empty\n"); //0�̶�� ������ ����ִٰ� ������ ���ֱ� ������ Ȯ���� �˷��ݴϴ�.
		return 0;
	}
	return pstack->stack[pstack->top];//stack ���� ���� ������ �ݴϴ�.
}

void clearstackchar(stackchar* pstack) 
{
	l = pstack->top; //�����ϴµ��� ���ڰ� �ٲ�� ������ �̸� �ű�ϴ�.
	for (i = 0; i <= l; i++) //pstack�� top+1��ŭ �ݺ��մϴ�.
	{
		x = popchar(pstack); //pstack�� �ڷḦ �����մϴ�.
	}
	pstack->top = -1;
}