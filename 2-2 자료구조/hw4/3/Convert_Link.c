#include "Stack_Link.h"
int y = 0; // ���� �޾Ƽ� ������ ���� ����

int main()
{
	stackhead* stackone = createstack(); //������ ����ϴ�.
	printf("Enter a number:");
	scanf("%d", &y);
	printf("\n\nThe binary number of %d: ", y);
	while (y != 1) //���� ���� 1�� �ɶ����� 2�� �����ϴ�.
	{
		push(stackone, y % 2); //�������� ���ÿ� �ֽ��ϴ�.
		y = y / 2;
	}
	printf("1"); //���� 1�� ���� ���������� ����ϸ� ���ϴ� �������� �˴ϴ�.
	while (isemptystack(stackone) == 0) //���ÿ� �ƹ��͵� �ȳ��������� ���þȿ� ���빰�� ���鼭 �н��ϴ�.
	{
		y = pop(stackone);
		printf("%d", y);
	}
	destroystack(stackone); //������ �����մϴ�.
}