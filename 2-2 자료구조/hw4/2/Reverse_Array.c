#include "Stack_Array.h"
int y = 0; // 수를 받아서 저장을 위한 변수
char c; //수또는 EOF를 받기 위한 변수

int main()
{

	stack* stackone = createstack(100); //스택을 만듭니다.
	printf("Enter a number : <Ctrl+D> to stop: ");
	while ((c = getchar()) != EOF) //EOF를 받을 수 있게 char로 받습니다.
	{
		getchar(); //버퍼를 비웁니다.
		y = (int)(c - 48); //char로 받은 수를 int로 고칩니다.
		printf("Enter a number : <Ctrl+D> to stop: ");
		push(stackone, y);
	}

	printf("\nThe List of numbers reversed:\n");

	while (isemptystack(stackone) == 0) //스택에 아무것도 안남을때까지 스택안에 내용물을 빼면서 읽습니다.
	{
		y = pop(stackone);
		printf("%d\n", y);
	}
	destroystack(stackone); //스택을 제거합니다.
	return 0;
}