#include "Stack_Array.h"
int y = 0; // ���� �޾Ƽ� ������ ���� ����
char c; //���Ǵ� EOF�� �ޱ� ���� ����

int main()
{

	stack* stackone = createstack(100); //������ ����ϴ�.
	printf("Enter a number : <Ctrl+D> to stop: ");
	while ((c = getchar()) != EOF) //EOF�� ���� �� �ְ� char�� �޽��ϴ�.
	{
		getchar(); //���۸� ���ϴ�.
		y = (int)(c - 48); //char�� ���� ���� int�� ��Ĩ�ϴ�.
		printf("Enter a number : <Ctrl+D> to stop: ");
		push(stackone, y);
	}

	printf("\nThe List of numbers reversed:\n");

	while (isemptystack(stackone) == 0) //���ÿ� �ƹ��͵� �ȳ��������� ���þȿ� ���빰�� ���鼭 �н��ϴ�.
	{
		y = pop(stackone);
		printf("%d\n", y);
	}
	destroystack(stackone); //������ �����մϴ�.
	return 0;
}