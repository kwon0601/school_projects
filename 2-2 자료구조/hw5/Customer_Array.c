#include "Queue_Array.h"

int selectnumber = 0; //In,out,exit선택을 저장하는 변수


int main()
{
	queue* queueone = createqueue(100);
	char str[100] = "abc";
	char* chartemp = NULL;


	while (selectnumber!=2) //selectnumber가 2가 되면 빠져나갑니다.
	{
		printf("In (0), out (1), exit (2) :");
		scanf("%d", &selectnumber);
		switch (selectnumber)
		{
		case 0: //In
			printf("\nCustomer : ");
			gets(str); //버퍼를 비웁니다.
			gets(str);
			enqueue(queueone, str);
			printf("\n");
			printqueue(queueone);
			break;
		case 1: //Out 
			if (isemptyqueue(queueone)) //queue가 비었을때를 처리합니다.
			{
				printf("\nQueue is empty !\n\n");
				printqueue(queueone);
			}
			else
			{
				printf("\nCustomer dequeued : ");
				chartemp = dequeue(queueone);
				printf("%s\n\n", chartemp);
				printqueue(queueone);
			}
			break;
		case 2: //exit
			break;
		default:
			printf("please write number 0,1or2\n");
			break;
		}
	}

	destroyqueue(queueone);
	return 0;
}