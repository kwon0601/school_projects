//��������� 2�� ���ٺ��� Stack_Link.h���� �Ʒ��� ���� �����߽��ϴ�.
//element > elementtwo 
//������ �̸�stack > stackchar, linkstack>linkstackchar
//��� ������ �̸� stackhead>stackheadchar, linkstackhead>linkstackheadchar
// ���� �̸� stacktemp>stacktempchar
// ���� x��� p�� �����߽��ϴ�.
//�Լ����� �ڿ� char�� ��� �ٿ����ϴ�.
//�̹� ����� ����(i,l),������ϵ��� �缱������ �ʾҽ��ϴ�.
typedef char elementtwo;//������ �ڷᰡ int ���� char�� �ٲ�����ϴ�.
//�� �̿ܿ��� �����մϴ�.

typedef struct linkstackchar {
	elementtwo content;
	struct linkstackchar* next; //���� stackchar�� �ּ�
}stackchar;

typedef struct linkstackheadchar { //elementtwo�� int�� �ƴ� �� �����Ƿ� �Ӹ��� ���� �����մϴ�.
	int content;
	stackchar* next; //���� stackchar�� �ּ�
}stackheadchar;

stackchar* stacktempchar = NULL; //�ӽ÷� stack�� �ּҰ��� ���� ����

elementtwo p = 0; //�Լ��� ���ϰ��� �ޱ� ���� ����,pop������ ����

stackheadchar* createstackchar()
{
	stackheadchar* pstackhead = (stackheadchar*)malloc(sizeof(stackheadchar)); //���α׷��� ����Ǵ� ���߿� �����ǹǷ� �����Ҵ��� �մϴ�.
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
		pstack->content = item; //���θ��� ���ÿ� content�� next�� �Է��մϴ�.
		pstack->next = pstackhead->next; //���� ���� ���� ������ next�� NULL�̿��� �ȵȰſ����ϴ�.
		pstackhead->next = pstack; //�Ӹ��� next�� content�� �����մϴ�.
		pstackhead->content++;
	}
}

elementtwo popchar(stackheadchar* pstackhead)
{
	stacktempchar = pstackhead->next; //����(���ַ��� �ϴ�) stackchar�� �ּҸ� stacktempchar�� �����մϴ�.
	x = pstackhead->next->content;//x�� ������ ������ ���� �����մϴ�.
	pstackhead->next = pstackhead->next->next;//������ 2��° ������ �ּҸ� �Ӹ��� �����մϴ�.
	free(stacktempchar); //�� ���� ������ ���۴ϴ�.
	pstackhead->content--;//head�� content���� 1�����ϴ�.
	return x;// ������ �־��� ���� �����մϴ�.
}

elementtwo topchar(stackheadchar* pstackhead)
{
	if (pstackhead->next == NULL)
	{
		printf("your stack is empty\n"); //0�̶�� ������ ����ִٰ� ������ ���ֱ� ������ Ȯ���� �˷��ݴϴ�.
		return 0;
	}
	return pstackhead->next->content; //���� ������ ������ �ݴϴ�.
}

int isemptystackchar(stackheadchar* pstackhead)
{
	if (pstackhead->content == 0) return 1; //empty�϶� �Դϴ�.
	return 0; //���빰�� ������ �Դϴ�.
}

int countstackitemchar(stackheadchar* pstackhead)
{
	return pstackhead->content; //���� ��� �ڷᰡ �ִ��� �����մϴ�.
}

void clearstackchar(stackheadchar* pstackhead)
{
	l = pstackhead->content;//�����ϴµ��� ���ڰ� �ٲ�� ������ �̸� �ű�ϴ�. //����
	for (i = 0; i < l; i++) //������ ����ŭ pop�� �����մϴ�.
	{
		x = popchar(pstackhead);
	}
}

void destroystackchar(stackheadchar* pstackhead)
{
	clearstackchar(pstackhead); //������ ���ϴ�.
	free(pstackhead); //�Ӹ��� ���ֹǷν� ������ �������ϴ�.
}
