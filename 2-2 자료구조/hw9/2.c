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

void take_data(int* data_array, int* size); //파일이름 입력받아 파일 내용을 저장. 1.c와 동일
void print_data(int* data_array, int size); //데이터를 출력하는 함수, 100개 이상의 자료가 대상이라 10개씩 출려되게 했습니다.
void data_swap(int* data_one, int* data_two);
void selection_sort(int* data_array, int size);
void insertion_sort(int* data_array, int size);
void bubble_sort(int* data_array, int size);
void quick_sort(int* data_array, int size);
void merge_sort(int* data_array, int size);

int main()
{
	int data_array[1000] = { 0, };
	int data_size = 0; //실제 데이터의 개수
	int choosing_method = 5;

	take_data(data_array, &data_size);//파일이름 입력받아 파일 내용을 저장

	while (4 < choosing_method || choosing_method < 0)
	{
		printf("\nChoose a sorting method to use (Selection : 0, Insertion : 1, Bubble : 2, Quick : 3, Merge : 4 )");
		scanf("%d", &choosing_method); //서칭 메서드를 입력받음
		switch (choosing_method)
		{
		case 0: //설렉션
		{
			selection_sort(data_array, data_size);
			break;
		}
		case 1:	//인설션
		{
			insertion_sort(data_array, data_size);
			break;
		}
		case 2: //버블
		{
			bubble_sort(data_array, data_size);
			break;
		}
		case 3:	//퀵
		{
			quick_sort(data_array, data_size);
			break;
		}
		case 4:	//머지
		{
			merge_sort(data_array, data_size);
			break;
		}
		default:
		{
			printf("Please write number 0, 1, 2, 3 or 4\n");
			break;
		}
		}
	}

	printf("The numbers sorted : ");
	print_data(data_array, data_size);

	return 0;
}

void take_data(int* data_array, int* size) //파일이름 입력받아 파일 내용을 저장. 1.c와 동일
{
	char file_name[50];
	char string_temp[200];

	printf("Input file name : ");
	scanf("%s", &file_name);
	FILE* file = fopen(file_name, "r");
	char* ptr = NULL;
	i = 0;

	while (!feof(file))
	{
		fgets(string_temp, 200, file);
		ptr = strtok(string_temp, " ");
		if (ptr != NULL) data_array[i++] = atoi(ptr);
		while (ptr != NULL)
		{
			ptr = strtok(NULL, " ");
			if (ptr != NULL) data_array[i++] = atoi(ptr);
		}
	}

	*size = i;
	fclose(file);
}

void print_data(int* data_array, int size) //데이터를 출력하는 함수
{
	for (i = 0; i < size-1; i++)
	{
		if (i % 10 == 9)
		{
			printf("%d\n", data_array[i]);
		}
		else 
		{
			printf("%d, ", data_array[i]);
		}
	}
	printf("%d\n", data_array[i]);
}

void data_swap(int* data_one, int* data_two)
{
	int data_temp = 0;
	data_temp = *data_one;
	*data_one = *data_two;
	*data_two = data_temp;
}

void selection_sort(int* data_array, int size)
{
	int minimum_data = 0;
	for (i = 0; i < size; i++)
	{
		for (j = i+1,minimum_data=i; j < size; j++)
		{
			if (data_array[minimum_data] > data_array[j])
				minimum_data= j;
		}
		data_swap(&data_array[i], &data_array[minimum_data]);
	}
}

void insertion_sort(int* data_array, int size)
{
	for (i = 1; i < size ; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (data_array[i] < data_array[j]) //삽입하려는 i번째 데이터보다 처음으로 만나는 큰 j번째 데이터 를 찾습니다.
			{
				int data_temp = data_array[i];
				int k=0;
				for (k = i-1; k >= j; k--) //새롭게 정렬되는 데이터보다 큰 거는 한칸씩 밉니다.
				{
					data_array[k + 1] = data_array[k];
				}
				data_array[j] = data_temp;
				break;
			}
		}
	}
}

void bubble_sort(int* data_array, int size)
{
	for (i = 0;i<size-1;i++)
	{
		for (j = size-1;i<j; j--)
		{
			if (data_array[j] < data_array[j - 1])
			{
				data_swap(&data_array[j], &data_array[j - 1]);
			}
		}
	}
}

void quick_sort(int* data_array, int size)
{
	if (size <= 1) return;

	int pivot = data_array[0];
	//pivot을 기준으로 좌우를 놔눕니다.
	i = 1; 
	j = size-1;
	while (i < j)
	{
		for (; i < j && data_array[i] <= pivot; i++);
		for (; i < j && data_array[j] >= pivot; j--);
		if (i < j)
			data_swap(&data_array[i], &data_array[j]);
	}

	//pivot을 중간으로 옮긴 뒤 pivot 좌우측을 정렬합니다. (pivot이 가장 큰 경우는 예외처리합니다.)
	
	if (data_array[i]<data_array[0])  //pivot이 가장 큰 숫자였을 경우 
	{ //예외처리를 해야하는 이유는 i(j)번째 값도 pivot보다 작으므로 i-1번째값과 pivot을 교환하면 안됩니다. 
		data_swap(&data_array[0], &data_array[i]);
		quick_sort(data_array, size-1);
		return ;
	}
	
	data_swap(&data_array[0], &data_array[i-1]);
	int pivot_number = i-1; //재귀호출을 하면 i가 바뀌므로 값을 저장합니다.

	quick_sort(data_array, pivot_number);//pivot 좌측을 정렬
	quick_sort(&data_array[pivot_number +1], size- pivot_number -1);//pivot 우측을 정렬
}

void merge_sort(int* data_array, int size)
{
	if (size <= 1) return;

	int center = size / 2;
	merge_sort(data_array, center); //앞부분을 정렬
	merge_sort(&data_array[center], size-center); //뒷부분을 정렬


	int* data_temp_arr = (int*)calloc(sizeof(int) * size, sizeof(int));
	if (data_temp_arr == NULL)
		return;

	int k = 0; 
	//두 배열을 병합합니다.
	for (i = 0, j = center; i < center&& j < size;k++)
	{
		if (data_array[i] > data_array[j])
		{
			data_temp_arr[k] = data_array[j];
			j++;
		}
		else
		{
			data_temp_arr[k] = data_array[i];
			i++;
		}
	}
	
	if (i == center)//왼쪽을 먼저 다 넣은 경우
	{
		while (k != size)
		{
			data_temp_arr[k] = data_array[j];
			j++;
			k++;
		}
	}
	else //오른쪽을 먼저 다 넣은 경우
	{
		while (k != size)
		{
			data_temp_arr[k] = data_array[i];
			i++;
			k++;
		}
	}

	for (i = 0; i < size; i++)
	{
		data_array[i]=data_temp_arr[i];
	}
	free(data_temp_arr);
}
