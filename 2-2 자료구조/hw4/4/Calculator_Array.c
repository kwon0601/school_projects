#include "Stack_Array.h" //int��
#include "Stack_Array2.h" //char��
#include <string.h>//strok

char str[100] = { '\0' }; //������ �ޱ� ���� ���ڿ�
char strcopy[100] = { '\0' }; //str�� (��)�� �����ؼ� �����ϴ� ���ڿ�
char* token = NULL; //strok�� ���� ������, �߰��� ���� ���ſ��� ���
char temp = 'a'; //posttfix ��ȯ�������� ���� ����
int slotcount = 0; //���ڿ��� ������ ���� ��ȣ�� ��׿��ִ��� ��Ÿ���� ����
int j = 0; //�ݺ����� ���,post�Ŀ��� ��궧 ���
int k = 0; //�ݺ����� ���,post�Ŀ��� ��궧 ���
int y = 0; //����ȿ ���˶� ���
int z = -1; //����ȿ ���˶� ���


int main()
{
	stackchar* stackone = createstackchar(100); //������ ����ϴ�.
	stackchar* stacktwo = createstackchar(100); //�� ������ postfix�� ����� �������� ȭ�鿡 ����� �� �ý��ۻ����ε� �����ؼ� ���߿� ����ϱ� �����Դϴ�.
	printf("Arithmetic Expression : ");
	gets(str);
	//-----------------------�Ʒ��� ��ȣ�� ������ ������ �������� �˻� �ϴ� �ڵ��Դϴ�.-----------------------------
	strcpy(strcopy, str); //strcopy�� str�� �����մϴ�.
	token = (char*)&strcopy;
	for (; *token != '\0'; token++)//strcopy���� ��ȣ�� �����մϴ�. token�� ���� �� ���̰� �ִ� ������ ���ο� ���� ���� ���� �׳� �����ϴ�.
	{
		if (*token == ')'|| *token == '(')//'('��')'�� ���ڿ��� �մ�ܼ� �ش� ��ȣ�� ���۴ϴ�.
		{
			strcpy(token, token + 1);
			token--;
		}
	}
	token = strtok(strcopy, " "); //���� �������� �ڸ��ϴ�.
	while (token != NULL) //���� ���ڿ��� ��ȣ�� ������ ��ȿ���� �˻��մϴ�.
	{
		y = 1;
		for (i = 0; i < strlen(token); i++) //���ڿ��� ���̸�ŭ push�� �մϴ�.
		{
			if (isemptystackchar(stackone) == 0&&(topchar(stackone)=='+'|| topchar(stackone) == '-' || topchar(stackone) == '*' || topchar(stackone) == '%' || topchar(stackone) == '/'))
			{
				if (token[i] == '+' || token[i] == '-' || token[i] == '*' || token[i] == '%' || token[i] == '/') //��ȣ�� ������ �Ŀ��� �����ڰ� �����ؼ� ������  �����Դϴ�.
				{
					printf("Error : Check the binary expression!\n");
					destroystackchar(stackone); //������ �����մϴ�.
					return 0;
				}
			}
			pushchar(stackone, token[i]);
			if ((int)token[i] - 48 < 0 || (int)token[i] - 48 > 9) y = 0; //���� ��ū�� ��� ���ڶ�� 1�� �˴ϴ�.
		}
		if (y == 1 && z == 1) //  ��ȣ�� ������ �Ŀ��� ���ڰ� �����ؼ� ������ �����Դϴ�.
		{
			printf("Error : Check the binary expression!\n");
			destroystackchar(stackone); //������ �����մϴ�.
			return 0;
		}
		if (y == 1) z = 1; // �̹� ���ڿ��� ���ڷθ� �����Ǿ����� �ƴ��� ����մϴ�.
		else z = 0;
		token = strtok(NULL, " ");
	}
	clearstackchar(stackone); //.���þ��� ���ϴ�.
	//-----------------------�Ʒ��� +-*/%,�����̿ܿ� ���� ��������. ��ȣ�� �˸����� �˻��ϴ� �ڵ��Դϴ�.-----------------------------
	strcpy(strcopy, str); //strcopy�� str�� �����մϴ�. 
	
	for (i = 0; i < strlen(strcopy); i++) //���ڿ��� ���̸�ŭ push�� �մϴ�.
	{
		pushchar(stackone, strcopy[i]);
		if (((int)strcopy[i] - 48 >= 0 && (int)strcopy[i] - 48 <= 9) || strcopy[i] == ' ' || strcopy[i] == '(' || strcopy[i] == ')' || strcopy[i] == '+' || strcopy[i] == '-' || strcopy[i] == '*' || strcopy[i] == '%' || strcopy[i] == '/');
		else //���� ���ڰ� ���ڳ� +-*%/()�� �ƴϸ� ��µ˴ϴ�.
		{
			printf("%c", strcopy[i]);
			printf("Error : input is must be number or +-*%%/ or brackets!\n");
			destroystackchar(stackone); //������ �����մϴ�.
			return 0;
		}
		if (strcopy[i] == '(') slotcount++; //�ִ� ���� '('�� ������ slotcoaunt�� 1���ϰ� ')'�� 1���ϴ�.
		else if (strcopy[i] == ')') slotcount--;
		if (slotcount < 0) // slotcount�� 0�̸�(-1)�� �Ǹ� '('���� ')'�� ���Դ� ���̹Ƿ� ���α׷��� �����մϴ�.
		{
			printf("Error : Check the pair of Parenthesis!\n");
			destroystackchar(stackone); //������ �����մϴ�.
			return 0;
		}
	}
	if (slotcount != 0) // ���ڿ��� �� �о��µ� slotcount�� 0�� �ƴ϶�� ��ȣ�� ¦�� ���� �����Ƿ� ���α׷��� �����մϴ�.
	{
		printf("Error : Check the pair of Parenthesis!\n");
		destroystackchar(stackone); //������ �����մϴ�.
		return 0;
	}
	//-----------------------�Ʒ��� �� ó�� ������ ����ϰ�, �� ���忡�� ���⸦ �����ϴ� �ڵ��Դϴ�.-----------------------------
	printf("Input : %s\nPost : ", str);
	token = (char *)&str;
	for (; *token != '\0'; token++)//str���� ���⸦ �����մϴ�. token�� ���� �� ���̰� �ִ� ������ ���ο� ���� ���� ���� �׳� �����ϴ�.
	{
		if (*token == ' ')//����� ���ڿ��� �մ�ܼ� �ش� ������ ���۴ϴ�.
		{
			strcpy(token, token + 1);
			token--;
		}
	}
	//-----------------------�Ʒ��� postfix�� ����ϰ�, ��°���� stacktwo�� ��� �ڵ��Դϴ�.-----------------------------
	clearstackchar(stackone); //.���þ��� ���ϴ�.
	for (j = 0; str[j]!='\0'; j++) //str�� ������ �ݺ� �մϴ�. 
	{
		if ((int)str[j] - 48 >= 0 && (int)str[j] - 48 <= 9)// ����� ��� �մϴ�.
		{
			printf(" %d", (int)str[j]-48 );
			pushchar(stacktwo, str[j]);
			while ((int)str[j+1] - 48 >= 0 && (int)str[j+1] - 48 <= 9) //�� ���� ���ڵ� ����� ���� �ƴҶ����� ��� ����մϴ�.
			{
				printf("%d", (int)str[++j] - 48);
				pushchar(stacktwo, str[j]);
			}
			pushchar(stacktwo, ' ');//������ ������ ���⸦ �־� ������ �� �ְ� �մϴ�.
		}
		else if (str[j] == '(')  // ���� ��ȣ��� '('�� ���ÿ� �ֽ��ϴ�.
		{
			pushchar(stackone, '(');
		}
		else if (str[j] == ')')  // �ݴ� ��ȣ��� '('���� pop�� �մϴ�.
		{
			while(topchar(stackone) != '(')
			{
				temp = popchar(stackone);
				printf(" %c", temp);
				pushchar(stacktwo, temp);
			}
			temp = popchar(stackone);
		}
		else //������
		{
			 if (str[j] == '*' || str[j] == '%' || str[j] == '/') //*�Ǵ� %�Ǵ�/���, ������ *�� %�� �� pop�ϰ�, push�մϴ�.
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
				 while (isemptystackchar(stackone) == 0 &&topchar(stackone)!='(') //���ÿ� �ƹ��͵� �ȳ��ų� '('�� ���������� pop�ϰ� �� ��, push�մϴ�.
				 {
					 temp = popchar(stackone);
					 printf(" %c", temp);
					 pushchar(stacktwo, temp);
				 }
				 pushchar(stackone, str[j]);
			 }
		}
	}
	while (isemptystackchar(stackone) == 0) //���ÿ� �ƹ��͵� �ȳ��������� pop�մϴ�.('('�� ǥ�þ��մϴ�)
	{
		temp = popchar(stackone);
		if (temp != '(')
		{
			printf(" %c", temp);
			pushchar(stacktwo, temp);
		}
	}
	clearstackchar(stackone); //���þ��� ���ϴ�.(�� �������� �� ������� �ϳ� ������ ���ؼ�)
	printf("\n");
	//-----------------------�Ʒ��� stacktwo���� stackone���� ���� �ű�� �ڵ��Դϴ�.-----------------------------
	while (isemptystackchar(stacktwo) == 0) //stacktwo���� ���� ���� �Ųٷ� �� �־ stackone���� �ű�鼭 ���� ������ �ٲߴϴ�.
	{
		temp = popchar(stacktwo);
		pushchar(stackone, temp);
	}
	//-----------------------�Ʒ��� postfix�� ����ؼ� ����ϴ� �ڵ��Դϴ�.-----------------------------
	stack* stackint = createstack(100); //int�� ������ ����ϴ�.
	for (i = 0; isemptystackchar(stackone)==0; i++)
	{
		temp = popchar(stackone);
		if (((int)temp - 48 >= 0 && (int)temp - 48 <= 9)) //�����϶�
		{
			y = (int)temp - 48;
			while (topchar(stackone) != ' ')  //���� ���� ���� �����̹Ƿ� �н��ϴ�.
			{
				temp = popchar(stackone);
				y=10*y+ (int)temp - 48;  //�̷��� ����ϸ� char�������� ���� �ִ� ���ڰ� ���������� ������ ���ڰ� �˴ϴ�.
			}
			push(stackint, y); //������ ���ڸ� y�� ����ϴ�.
		}
		else if (temp == ' '); //���� �϶� �ƹ��͵� ���մϴ�.
		//�����ڶ�� ���� 2���� ���� ���� ����� �� �ٽ� ������ �˴ϴ�.
		else if (temp == '+') //���ϱ� �϶�
		{
			j=pop(stackint);
			k = pop(stackint);
			push(stackint, k + j);
		}
		else if (temp == '-') //���� �϶�
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k - j);
		}
		else if (temp == '*') //���ϱ� �϶�
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k * j);
		}
		else if (temp == '%') //mod �϶�
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k % j);
		}
		else if (temp == '/') //������ �϶�
		{
			j = pop(stackint);
			k = pop(stackint);
			push(stackint, k / j);
		}
	}
	j = pop(stackint); 
	printf("Result : %d\n", j);
	destroystack(stackint); //������ �����մϴ�.
	destroystackchar(stackone); 
	destroystackchar(stacktwo);
	return 0;
}