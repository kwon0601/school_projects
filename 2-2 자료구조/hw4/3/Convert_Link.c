#include "Stack_Link.h"
int y = 0; // 수를 받아서 저장을 위한 변수

int main()
{
	stackhead* stackone = createstack(); //스택을 만듭니다.
	printf("Enter a number:");
	scanf("%d", &y);
	printf("\n\nThe binary number of %d: ", y);
	while (y != 1) //받은 수를 1이 될때까지 2로 나눕니다.
	{
		push(stackone, y % 2); //나머지를 스택에 넣습니다.
		y = y / 2;
	}
	printf("1"); //이제 1과 받은 나머지들을 출력하면 원하는 이진수가 됩니다.
	while (isemptystack(stackone) == 0) //스택에 아무것도 안남을때까지 스택안에 내용물을 빼면서 읽습니다.
	{
		y = pop(stackone);
		printf("%d", y);
	}
	destroystack(stackone); //스택을 제거합니다.
}