//collision대책은 open addressing 그 중 linear probe방법을 사용했습니다.
//(충돌이 일어날시 그다음 해쉬값에 넣는 방법)
#pragma warning(disable:4996)

#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _string_h
#define _string_h
#include <string.h>
#endif

#ifndef _stdlib_h
#define _stdlib_h
#include <stdlib.h>
#endif

#ifndef _repeat_variable
#define _repeat_variable1
int i = 0; //반복문을 위한 변수
int j = 0;
#endif

void take_data( int* data_array, int* size ); //파일이름 입력받아 파일 내용을 저장
void hashing_numbers(int* data_array, int data_size, int*** hash_array); //배열에 있는 숫자를 해쉬로 전환해서 기록
void searching_hash(int** p_hash_array, int find_number); //찾고자 하는 숫자의 해쉬가 있는지 확인해서 결과 출력
void selection_sort(int* data_array, int size); //2.c에서와 동일합니다.
int binary_search(int* data_array, int size, int target_number); //찾는 값이 있는지 반환하는 함수

int main()
{
	int data_array[1000] = { 0, };
	int data_size = 0; //실제 데이터의 개수
	int find_number = 0;
	int choosing_method = 0;
	int** p_hash_array = NULL;
	int binary_result = 0;

	take_data(data_array, &data_size);//파일이름 입력받아 파일 내용을 저장

	printf("Target integer : "); 
	scanf("%d", &find_number);

	while (choosing_method != 1 && choosing_method != 2)
	{
		printf("\nChoose a searching method (Binary : 1, HASH : 2) : ");
		scanf("%d", &choosing_method);
		switch (choosing_method)
		{
		case 1: //바이너리로 찾기(정렬 필요)
			selection_sort(data_array, data_size);
			binary_result = binary_search(data_array, data_size, find_number); //찾는 값이 있는지 반환하는 함수
			if(binary_result)
				printf("%d is found!", find_number);
			else
				printf("%d is not found!", find_number);
			break;
		case 2: //해쉬로 찾기
		{
			hashing_numbers(data_array, data_size, &p_hash_array);//배열에 있는 숫자를 해쉬로 전환해서 기록
			searching_hash(p_hash_array, find_number); //찾고자 하는 숫자의 해쉬가 있는지 확인해서 결과 출력
			break;
		}
		default:
		{
			printf("Please write number 1 or 2\n");
			break;
		}
		}
	}
	return 0;
}

void take_data(int* data_array, int* size) //파일이름 입력받아 파일 내용을 저장
{
	char file_name[50];
	char string_temp[50];

	printf("Input file name : ");
	scanf("%s", &file_name);
	FILE* file=fopen(file_name,"r");
	char* ptr = NULL;
	i = 0;

	while (!feof(file))
	{
		fgets(string_temp, 50, file);
		ptr = strtok(string_temp, " ");
		if (ptr != NULL) data_array[i++] = atoi(ptr);
		while (ptr != NULL)
		{
			ptr = strtok(NULL, " ");
			if (ptr!=NULL) data_array[i++] = atoi(ptr);
		}
	}

	*size = i ;
	fclose(file);
}

void hashing_numbers(int* data_array, int data_size, int*** hash_array) //배열에 있는 숫자를 해쉬로 전환해서 기록
{
	int hash = 0;

	int* hashed_data = (int*)calloc(1001 * sizeof(int), sizeof(int));
	if (hashed_data == NULL)
		return ;

	for (i = 0; i < data_size; i++)
	{
		if (data_array[i] == 0) //데이터가 0인 경우는 hashed_data 배열에 데이터 값인 0을 입력해도 변화가 없으므로 따로 1001번째 칸을 0전용으로 씁니다.
		{
			*(hashed_data+1001) = 1;
		}
		else
		{
			hash = data_array[i] % 1000;
			while (*(hashed_data+hash) != 0)
			{
				hash=(hash+1)%1000;
			}
			*(hashed_data+hash) = data_array[i];
		}
	}
	*hash_array = &hashed_data;
}

void searching_hash(int** p_hash_array, int find_number) //찾고자 하는 숫자의 해쉬가 있는지 확인해서 결과 출력
{
	int result = 0;

	if (find_number == 0)
	{
		if (*(*p_hash_array+1001) == 1) result=1;
	}
	else 
	{
		i = 0;
		while (result==0)
		{
			if (*(*p_hash_array + (find_number + i) % 1000) == find_number) result = 1;
			else if(*(*p_hash_array + (find_number + i) % 1000) == 0) break;
			i++;
		}
	}
	
	if (result == 1)
		printf("%d is found!", find_number);
	else
		printf("%d is not found!", find_number);
	
}

void selection_sort(int* data_array, int size)
{
	int minimum_data = 0;
	int data_temp = 0;
	for (i = 0; i < size; i++)
	{
		for (j = i + 1, minimum_data = i; j < size; j++)
		{
			if (data_array[minimum_data] > data_array[j])
				minimum_data = j;
		}
		data_temp = data_array[i];
		data_array[i] = data_array[minimum_data];
		data_array[minimum_data] = data_temp;
	}
}

int binary_search(int* data_array, int size , int target_number) //찾는 값이 있는지 반환하는 함수
{
	if (size == 1) //쪼개고 쪼개어 1개만 남았을 경우
	{
		if (data_array[0] == target_number)
			return 1;
		else
			return 0;
	}
	if (data_array[size / 2] == target_number) return 1;
	if (data_array[size / 2] > target_number)
	{
		return binary_search(data_array, size/2, target_number);
	}
	else
	{
		return binary_search(data_array+size/2, size-size / 2, target_number);
	}
}