#include "Stack_Array.h" //int형
#include "Stack_Array2.h" //char형
#include <string.h>//strok

char str[100] = { '\0' }; //수식을 받기 위한 문자열
char strcopy[100] = { '\0' }; //str을 (와)를 제외해서 복사하는 문자열
char* token = NULL; //strok에 쓰일 포인터, 중간에 띄어쓰기 제거에도 사용
char temp = 'a'; //posttfix 변환가정에서 쓰는 변수
int slotcount = 0; //문자열을 읽을때 여는 괄호가 몇개쌓여있는지 나타내는 변수
int j = 0; //반복문에 사용,post식에서 계산때 사용
int k = 0; //반복문에 사용,post식에서 계산때 사용
int y = 0; //식유효 점검때 사용
int z = -1; //식유효 점검때 사용


int main()
{
	stackchar* stackone = createstackchar(100); //스택을 만듭니다.
	stackchar* stacktwo = createstackchar(100); //이 스택은 postfix로 만드는 과정에서 화면에 출력할 때 시스템상으로도 저장해서 나중에 계산하기 위함입니다.
	printf("Arithmetic Expression : ");
	gets(str);
	//-----------------------아래는 괄호를 제외한 문자의 완전성을 검사 하는 코드입니다.-----------------------------
	strcpy(strcopy, str); //strcopy에 str을 복사합니다.
	token = (char*)&strcopy;
	for (; *token != '\0'; token++)//strcopy에서 괄호를 제외합니다. token은 현재 안 쓰이고 있는 변수라서 새로운 변수 선언 없이 그냥 썻습니다.
	{
		if (*token == ')'|| *token == '(')//'('나')'면 문자열을 앞당겨서 해당 괄호를 없앱니다.
		{
			strcpy(token, token + 1);
			token--;
		}
	}
	token = strtok(strcopy, " "); //띄어쓰기 기준으로 자릅니다.
	while (token != NULL) //받은 문자열의 괄호를 제외한 유효성을 검사합니다.
	{
		y = 1;
		for (i = 0; i < strlen(token); i++) //문자열의 길이만큼 push를 합니다.
		{
			if (isemptystackchar(stackone) == 0&&(topchar(stackone)=='+'|| topchar(stackone) == '-' || topchar(stackone) == '*' || topchar(stackone) == '%' || topchar(stackone) == '/'))
			{
				if (token[i] == '+' || token[i] == '-' || token[i] == '*' || token[i] == '%' || token[i] == '/') //괄호를 제외한 식에서 연산자가 연속해서 나오면  에러입니다.
				{
					printf("Error : Check the binary expression!\n");
					destroystackchar(stackone); //스택을 제거합니다.
					return 0;
				}
			}
			pushchar(stackone, token[i]);
			if ((int)token[i] - 48 < 0 || (int)token[i] - 48 > 9) y = 0; //받은 토큰이 모두 숫자라면 1이 됩니다.
		}
		if (y == 1 && z == 1) //  괄호를 제외한 식에서 숫자가 연속해서 나오면 에러입니다.
		{
			printf("Error : Check the binary expression!\n");
			destroystackchar(stackone); //스택을 제거합니다.
			return 0;
		}
		if (y == 1) z = 1; // 이번 문자열이 숫자로만 구성되었는지 아닌지 기록합니다.
		else z = 0;
		token = strtok(NULL, " ");
	}
	clearstackchar(stackone); //.스택안을 비웁니다.
	//-----------------------아래는 +-*/%,숫자이외에 것이 들어오는지. 괄호가 알맞은지 검사하는 코드입니다.-----------------------------
	strcpy(strcopy, str); //strcopy에 str을 복사합니다. 
	
	for (i = 0; i < strlen(strcopy); i++) //문자열의 길이만큼 push를 합니다.
	{
		pushchar(stackone, strcopy[i]);
		if (((int)strcopy[i] - 48 >= 0 && (int)strcopy[i] - 48 <= 9) || strcopy[i] == ' ' || strcopy[i] == '(' || strcopy[i] == ')' || strcopy[i] == '+' || strcopy[i] == '-' || strcopy[i] == '*' || strcopy[i] == '%' || strcopy[i] == '/');
		else //들어온 문자가 숫자나 +-*%/()가 아니면 출력됩니다.
		{
			printf("%c", strcopy[i]);
			printf("Error : input is must be number or +-*%%/ or brackets!\n");
			destroystackchar(stackone); //스택을 제거합니다.
			return 0;
		}
		if (strcopy[i] == '(') slotcount++; //넣는 와중 '('를 넣으면 slotcoaunt를 1더하고 ')'는 1뺍니다.
		else if (strcopy[i] == ')') slotcount--;
		if (slotcount < 0) // slotcount가 0미만(-1)이 되면 '('전에 ')'가 나왔단 뜻이므로 프로그램을 종료합니다.
		{
			printf("Error : Check the pair of Parenthesis!\n");
			destroystackchar(stackone); //스택을 제거합니다.
			return 0;
		}
	}
	if (slotcount != 0) // 문자열을 다 읽었는데 slotcount가 0이 아니라면 괄호가 짝이 맞지 않으므로 프로그램을 종료합니다.
	{
		printf("Error : Check the pair of Parenthesis!\n");
		destroystackchar(stackone); //스택을 제거합니다.
		return 0;
	}
	//-----------------------아래는 맨 처음 문장을 출력하고, 그 문장에서 띄어쓰기를 제외하는 코드입니다.-----------------------------
	printf("Input : %s\nPost : ", str);
	token = (char *)&str;
	for (; *token != '\0'; token++)//str에서 띄어쓰기를 제외합니다. token은 현재 안 쓰이고 있는 변수라서 새로운 변수 선언 없이 그냥 썻습니다.
	{
		if (*token == ' ')//띄어쓰기면 문자열을 앞당겨서 해당 공백을 없앱니다.
		{
			strcpy(token, token + 1);
			token--;
		}
	}
	//-----------------------아래는 postfix를 출력하고, 출력결과를 stacktwo에 담는 코드입니다.-----------------------------
	clearstackchar(stackone); //.스택안을 비웁니다.
	for (j = 0; str[j]!='\0'; j++) //str에 끝까지 반복 합니다. 
	{
		if ((int)str[j] - 48 >= 0 && (int)str[j] - 48 <= 9)// 수라면 출력 합니다.
		{
			printf(" %d", (int)str[j]-48 );
			pushchar(stacktwo, str[j]);
			while ((int)str[j+1] - 48 >= 0 && (int)str[j+1] - 48 <= 9) //이 다음 문자도 수라면 수가 아닐때까지 계속 출력합니다.
			{
				printf("%d", (int)str[++j] - 48);
				pushchar(stacktwo, str[j]);
			}
			pushchar(stacktwo, ' ');//숫자의 끝에는 띄어쓰기를 넣어 구별할 수 있게 합니다.
		}
		else if (str[j] == '(')  // 여는 괄호라면 '('를 스택에 넣습니다.
		{
			pushchar(stackone, '(');
		}
		else if (str[j] == ')')  // 닫는 괄호라면 '('까지 pop을 합니다.
		{
			while(topchar(stackone) != '(')
			{
				temp = popchar(stackone);
				printf(" %c", temp);
				pushchar(stacktwo, temp);
			}
			temp = popchar(stackone);
		}
		else //연산자
		{
			 if (str[j] == '*' || str[j] == '%' || str[j] == '/') //*또는 %또는/라면, 스택의 *와 %를 다 pop하고, push합니다.
				{
					 while (isemptystackchar(stackone) == 0&&(topchar(stackone) == '*' || topchar(stackone) == '%' || topchar(stackone) == '/'))
					 {
						 temp = popchar(stackone);
						 printf(" %c", temp);
						 pushchar(stacktwo, temp);
					 }
					 pushchar(stackone, str[j]);
				}
			 else
			 {
				 while (isemptystackchar(stackone) == 0 &&topchar(stackone)!='(') //스택에 아무것도 안남거나 '('를 만날때까지 pop하고 그 후, push합니다.
				 {
					 temp = popchar(stackone);
					 printf(" %c", temp);
					 pushchar(stacktwo, temp);
				 }
				 pushchar(stackone, str[j]);
			 }
		}
	}
	while (isemptystackchar(stackone) == 0) //스택에 아무것도 안남을때까지 pop합니다.('('는 표시안합니다)
	{
		temp = popchar(stackone);
		if (temp != '(')
		{
			printf(" %c", temp);
			pushchar(stacktwo, temp);
		}
	}
	clearstackchar(stackone); //스택안을 비웁니다.(위 과정에서 다 비워지긴 하나 만약을 위해서)
	printf("\n");
	//-----------------------아래는 stacktwo에서 stackone으로 식을 옮기는 코드입니다.-----------------------------
	while (isemptystackchar(stacktwo) == 0) //stacktwo에는 현재 식이 거꾸로 들어가 있어서 stackone으로 옮기면서 원래 순서로 바꿉니다.
	{
		temp = popchar(stacktwo);
		pushchar(stackone, temp);
	}
	//-----------------------아래는 postfix를 계산해서 출력하는 코드입니다.-----------------------------
	stack* stackint = createstack(100); //int형 스택을 만듭니다.
	for (i = 0; isemptystackchar(stackone)==0; i++)
	{
		temp = popchar(stackone);
		if (((int)temp - 48 >= 0 && (int)temp - 48 <= 9)) //숫자일때
		{
			y = (int)temp - 48;
			while (topchar(stackone) != ' ')  //띄어쓰기 이전 까지 숫자이므로 읽습니다.
			{
				temp = popchar(stackone);
				y=10*y+ (int)temp - 48;  //이렇게 계산하면 char여러개에 걸쳐 있던 숫자가 최종적으로 온전한 숫자가 됩니다.
			}
			push(stackint, y); //온전한 숫자를 y에 담습니다.
		}
		else if (temp == ' '); //띄어쓰기 일때 아무것도 안합니다.
		//연산자라면 이전 2개의 값을 꺼내 계산한 뒤 다시 넣으면 됩니다.
		else if (temp == '+') //더하기 일때
		{
			j=pop(stackint);
			k = pop(stackint);
			push(stackint, k + j);
		}
		else if (temp == '-') //빼기 일때
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k - j);
		}
		else if (temp == '*') //곱하기 일때
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k * j);
		}
		else if (temp == '%') //mod 일때
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k % j);
		}
		else if (temp == '/') //나누기 일때
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k / j);
		}
	}
	j = pop(stackint); 
	printf("Result : %d\n", j);
	destroystack(stackint); //스택을 제거합니다.
	destroystackchar(stackone); 
	destroystackchar(stacktwo);
	return 0;
}