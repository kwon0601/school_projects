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

#include "Ad_Matrix3.h"

#ifndef _repeat_variable
#define _repeat_variable1
int i = 0; //반복문을 위한 변수
int j = 0;
#endif

int cost = 0;

void print_conneted(m_graph* pm_graph); //연결된 vertex를 출력하는 함수
void print_minimum_spanning_tree(m_graph* pm_graphchar, char start_vertex); //minimum_spanning_tree를 출력하는 함수
void print_shortest_path(m_graph* pm_graphchar, char start_vertex); //shortest_path를 출력하는 함수

int main()
{
	m_graph* m_graph_one = m_create_graph(char_compare, char_free);

	char file_name[50];
	char string_temp[50] = "-"; //파일에서 정보를 받기 위한 변수
	char start_vertex = '-'; //파일에서 읽어 온 시작 지점
	char from_vertex = '-';
	char to_vertex = '-';
	int add_result = 0;

	printf("File name of a graph : ");
	scanf("%s", &file_name);
	
	FILE* file = fopen(file_name, "r");

	fgets(string_temp, 50, file);
	char* ptr = strtok(string_temp, " ");
	start_vertex = *ptr;

	while (!feof(file))
	{
		fgets(string_temp, 50, file);
		ptr = strtok(string_temp, " ");

		//한줄중 이전 문자를 처리
		char* pchar_temp = (char*)malloc(sizeof(char));
		from_vertex = *ptr;
		*pchar_temp = from_vertex;
		if (!pchar_temp)
			return 0;
		add_result = m_add_vertex(m_graph_one, (void*)pchar_temp);
		ptr = strtok(NULL, " ");
		if (add_result == 0) free(pchar_temp); //만약 vertex가 이미 있으면 지금 free를 해주어야합니다.

		//한줄중 이후 문자를 처리
		char* pchar_temp2 = (char*)malloc(sizeof(char));
		to_vertex = *ptr;
		*pchar_temp2 = to_vertex;
		if (!pchar_temp2)
			return 0;
		add_result = m_add_vertex(m_graph_one, (void*)pchar_temp2);
		if (add_result == 0) free(pchar_temp2);
		ptr = strtok(NULL, " ");

		cost = atoi(ptr);

		m_add_edge(m_graph_one, (void*)&from_vertex, (void*)&to_vertex,cost);
		m_add_edge(m_graph_one, (void*)&to_vertex, (void*)&from_vertex,cost);
	}
	print_conneted(m_graph_one);
	print_minimum_spanning_tree(m_graph_one,start_vertex);
	print_shortest_path(m_graph_one, start_vertex);

	printf("\n");
	fclose(file);
	m_destroy_graph(m_graph_one);
	return 0;
}

void print_conneted(m_graph* pm_graph) //연결된 vertex를 출력하는 함수
{
	printf("The graph is :\n");
	memset(pm_graph->processed, 0, 20 * sizeof(int));
	for (j = 0; j < pm_graph->count; j++)
	{
		void* target_content = NULL;

		for (i = 0; i < pm_graph->count; i++)// target_content의 초기값을 처리안한 vertex 중  하나로 합니다.
		{
			if (pm_graph->processed[i] == 0)
			{
				target_content = pm_graph->content[i];
				break;
			}
		}

		for (i = 1; i < pm_graph->count; i++)// 처리를 안한 vertex중 가장 작은 알파벳vertex를 target_content가 가리키게 합니다.
		{
			if ((pm_graph->compare(target_content, pm_graph->content[i])) > 0 && pm_graph->processed[i] == 0)
				target_content = pm_graph->content[i];
		}

		for (i = 0; i < pm_graph->count; i++)// pogressed를 1로합니다.
		{
			if ((pm_graph->compare(target_content, pm_graph->content[i])) == 0)
			{
				pm_graph->processed[i] = 1;
				break;
			}
		}

		printf("%c : ", *(char*)target_content);
		int k = 0;
		for (k = 0; k < pm_graph->count; k++)
		{
			if (pm_graph->arc[i][k])
			{
				printf("%c (%d) ",*(char*)pm_graph->content[k], pm_graph->arc[i][k]);
			}
		}
		

		printf("\n");
	}
}

void print_minimum_spanning_tree(m_graph* pm_graph,char start_vertex)
{
	printf("\nMinimum spanning tree : \n");
	for (i = 0; i < pm_graph->count; i++) 
	{
		if (start_vertex==*(char*)pm_graph->content[i]) break;
	}

	int involved_vertex[20] = { 0, };//tree에 속한 vertex를 저장
	for (j = 0; j < 20; j++)
	{
		involved_vertex[j] = -1;
	}
	involved_vertex[0] = i;
	int cost_temp = 0;
	char* from_vertex=NULL;
	char* to_vertex=NULL;
	int involved_count=1;
	int count_temp = 0;

	while (1)
	{
		cost_temp = 0;
		for (i = 0; involved_vertex[i] != -1; i++)//적절한 arc를 찾습니다.
		{
			for (j = 0; j < pm_graph->count; j++)
			{
				if (pm_graph->arc[involved_vertex[i]][j] != 0 && (cost_temp == 0 || (cost_temp > pm_graph->arc[involved_vertex[i]][j])))
				{
					int k = 0;
					int check = 0;
					for (; k < 20; k++) //이전에 추가된 vertex인지 확인합니다.
					{
						if (involved_vertex[k] == j) check = 1;
					}
					if (check == 0)
					{
						cost_temp = pm_graph->arc[involved_vertex[i]][j];
						from_vertex = (char*)pm_graph->content[involved_vertex[i]];
						to_vertex = (char*)pm_graph->content[j];
						count_temp= j; //involved_vertex는 for문 밖에서 수정해야 하므로 count_temp에 임시로 넣어둡니다.
					}
				}
			}
		}
		involved_vertex[involved_count] = count_temp;
		if (cost_temp == 0) break; //arc가 없다면 모든 vertex를 포함했다는 뜻이므로 나갑니다.
		involved_count++;
		printf("%c %c (%d)\n", *from_vertex, *to_vertex, cost_temp);
	}

}

void print_shortest_path(m_graph* pm_graph, char start_vertex) //shortest_path를 출력하는 함수
{
	printf("\nShortest path from %c : \n",start_vertex);
	for (i = 0; i < pm_graph->count; i++)
	{
		if (start_vertex == *(char*)pm_graph->content[i]) break;
	}

	int involved_vertex[20] = { 0, };//경로 체크를 완료한 vertex를 저장

	for (j = 0; j < 20; j++)
	{
		involved_vertex[j] = -1;
	}
	involved_vertex[i] = 1;
	int checking_vertex = i;
	int lowest_cost = 10000;
	int next_temp = -1;

	int cost[20] = { 0 ,}; //start_vertex에서 n번째 vertex까지 비용
	for (j = 0; j < 20; j++)
	{
		cost[j] = 1000;
	}
	cost[i] = 0; //start_vertex자신
	int k = 0;
	for (k = 0; k < pm_graph->count; k++)
	{
		for (lowest_cost=1000,next_temp=-1, j = 0; j < pm_graph->count; j++)
		{
			if ((pm_graph->arc[checking_vertex][j])!=0&&involved_vertex[j]==-1) //checking_vertex->j 길이 있고,j가 아직 속하지 않았다면,
			{
				if (cost[j] > cost[checking_vertex] + pm_graph->arc[checking_vertex][j])
					cost[j] = cost[checking_vertex] + pm_graph->arc[checking_vertex][j];
				if (lowest_cost > pm_graph->arc[checking_vertex][j])
				{
					lowest_cost = pm_graph->arc[checking_vertex][j];
					next_temp = j;
				}
			}
		}
		if(next_temp!=-1)
			involved_vertex[next_temp] = 1;
		checking_vertex = next_temp;

		for (j = 0; j < pm_graph->count; j++)// 출력
		{
			if(j!=i)
				printf("%c %c : %d\n",*(char*)(pm_graph->content[i]), *(char*)(pm_graph->content[j]),cost[j]);
		}
		printf("\n");
	}
}