# include <stdio.h>
# include <stdlib.h> //�����Ҵ�
typedef int element;

typedef struct {
	element* stack; //�ڷᰡ ����� ���� �ּ�
	int size; //�ִ� �ڷ� ����
	int top; //���� �� ���� �ڷᰡ �ִ���(1���ִٸ� 0)
}stack;
int i = 0; //�ݺ����� ���� ����
int l = 0; //clearstack�� ���� ����
int x = 0; //�Լ��� ���ϰ��� �ޱ� ���� ����

stack* createstack(int size)
{
	stack* pstack=(stack*)malloc(sizeof(stack)); //���α׷��� ����Ǵ� ���߿� �����ǹǷ� �����Ҵ��� �մϴ�.
	if (pstack == NULL)
		return NULL;
	pstack->stack = (element*)malloc(sizeof(element)*size); //element �迭�� size��ŭ ���� �� �ִ� ������ �޾� �� �ּҸ� stack�� ����մϴ�.
	if (pstack->stack == NULL)
	{
		free(pstack);
		return NULL;
	}
	pstack->size = size;
	pstack->top = -1;
	return pstack;
}

void destroystack(stack* pstack)
{
	free(pstack->stack); //size�� ������ ����̹Ƿ� ���� ���󿡼��� ���ǹ��� �ʿ���ٰ� �����ؼ� �ȳ־����ϴ�.
	free(pstack);
}

int isfullstack(stack * pstack)
{
	if (pstack->size > (pstack->top)+1) return 0;// ������ �����ִٸ� 0(false)�� �����մϴ�.
	return 1; // ������ ���ٸ� 1(true)�� �����մϴ�.
}

int isemptystack(stack* pstack)
{
	if (pstack->top==-1) return 1;// ���빰�� ���ٸ� 1(true)�� �����մϴ�.
	return 0; // ���빰�� �ִٸ� 0(false)�� �����մϴ�.
}

int countstackitem(stack* pstack)//���þȿ� �� ���� ���빰�� �ִ��� Ȯ���մϴ�.
{
	return (pstack->top)+1; 
}

void push(stack* pstack, element item)
{
	x = isfullstack(pstack);
	if (x == 1) //�ڷᰡ ���� �ִ��� Ȯ���մϴ�.
	{
		printf("your stack is full,so you can't push\n");
	}
	else
	{
		pstack->stack[++pstack->top] = item;
	}
}

element pop(stack* pstack)
{
	x = isemptystack(pstack);
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

element top(stack* pstack)
{
	x = isemptystack(pstack);
	if (x == 1) //�ڷᰡ ������� Ȯ���մϴ�.
	{
		printf("your stack is empty\n"); //0�̶�� ������ ����ִٰ� ������ ���ֱ� ������ Ȯ���� �˷��ݴϴ�.
		return 0;
	}
	return pstack->stack[pstack->top];//stack ���� ���� ������ �ݴϴ�.
}

void clearstack(stack* pstack)
{
	l = pstack->top; //�����ϴµ��� ���ڰ� �ٲ�� ������ �̸� �ű�ϴ�.
	for (i = 0; i <= l; i++) //pstack�� top+1��ŭ �ݺ��մϴ�.
	{
		x = pop(pstack); //pstack�� �ڷḦ �����մϴ�.
	}
	pstack->top = -1;
}