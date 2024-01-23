# include <stdio.h>
# include <stdlib.h> //�����Ҵ�
typedef int element;

typedef struct linkstack{
	element content;
	struct linkstack* next; //���� stack�� �ּ�
}stack;

typedef struct linkstackhead { //element�� int�� �ƴ� �� �����Ƿ� �Ӹ��� ���� �����մϴ�.
	int content;
	stack* next; //���� stack�� �ּ�
}stackhead;

stack* stacktemp=NULL; //�ӽ÷� stack�� �ּҰ��� ���� ����

int i = 0; //�ݺ����� ���� ����
int l = 0; //clearstack�� ���� ����
element x = 0; //�Լ��� ���ϰ��� �ޱ� ���� ����,pop������ ����


stackhead* createstack()
{
	stackhead* pstackhead = (stackhead*)malloc(sizeof(stackhead)); //���α׷��� ����Ǵ� ���߿� �����ǹǷ� �����Ҵ��� �մϴ�.
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
		pstack->content = item; //���θ��� ���ÿ� content�� next�� �Է��մϴ�.
		pstack->next = pstackhead->next; //���� ���� ���� ������ next�� NULL�̿��� �ȵȰſ����ϴ�.
		pstackhead->next = pstack; //�Ӹ��� next�� content�� �����մϴ�.
		pstackhead->content++;
	}
}

element pop(stackhead* pstackhead)
{
	stacktemp = pstackhead->next; //����(���ַ��� �ϴ�) stack�� �ּҸ� stacktemp�� �����մϴ�.
	x = pstackhead->next->content;//x�� ������ ������ ���� �����մϴ�.
	pstackhead->next = pstackhead->next->next;//������ 2��° ������ �ּҸ� �Ӹ��� �����մϴ�.
	free(stacktemp); //�� ���� ������ ���۴ϴ�.
	pstackhead->content--;//head�� content���� 1�����ϴ�.
	return x;// ������ �־��� ���� �����մϴ�.
}

element top(stackhead* pstackhead)
{
	if (pstackhead->next == NULL)
	{
		printf("your stack is empty\n"); //0�̶�� ������ ����ִٰ� ������ ���ֱ� ������ Ȯ���� �˷��ݴϴ�.
		return 0;
	}
	return pstackhead->next->content; //���� ������ ������ �ݴϴ�.
}

int isemptystack(stackhead* pstackhead)
{
	if (pstackhead->content == 0) return 1; //empty�϶� �Դϴ�.
	return 0; //���빰�� ������ �Դϴ�.
}

int countstackitem(stackhead* pstackhead)
{
	return pstackhead->content; //���� ��� �ڷᰡ �ִ��� �����մϴ�.
}

void clearstack(stackhead* pstackhead)
{
	l = pstackhead->content;//�����ϴµ��� ���ڰ� �ٲ�� ������ �̸� �ű�ϴ�. //����
	for (i = 0; i < l; i++) //������ ����ŭ pop�� �����մϴ�.
	{
		x=pop(pstackhead);
	}
}

void destroystack(stackhead* pstackhead)
{
	clearstack(pstackhead); //������ ���ϴ�.
	free(pstackhead); //�Ӹ��� ���ֹǷν� ������ �������ϴ�.
}
