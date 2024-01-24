#pragma warning(disable:4996)

#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _string_h
#define _string_h
#include <string.h>
#endif

#include "Ad_Matrix.h"

#ifndef _repeat_variable
#define _repeat_variable1
int i = 0; //반복문을 위한 변수
int j = 0;
#endif

void print_conneted(m_graph* pm_graph); //연결된 vertex를 출력하는 함수

int main()
{
	m_graph* m_graph_one = m_create_graph(char_compare,char_free);

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
		add_result=m_add_vertex(m_graph_one, (void*)pchar_temp);
		ptr = strtok(NULL, " ");
		if (add_result == 0) free(pchar_temp); //만약 vertex가 이미 있으면 지금 free를 해주어야합니다.

		//한줄중 이후 문자를 처리, edge삽입
		char* pchar_temp2 = (char*)malloc(sizeof(char));
		to_vertex = *ptr;
		*pchar_temp2 = to_vertex;
		if (!pchar_temp2)
			return 0;
		add_result = m_add_vertex(m_graph_one, (void*)pchar_temp2);
		m_add_edge(m_graph_one, (void*)&from_vertex, (void*)&to_vertex);
		if (add_result == 0) free(pchar_temp2);
	}
	print_conneted(m_graph_one);

	printf("\nDepth First Traversa : ");
	m_depth_traversal((void*)&start_vertex,m_graph_one,print_char);

	printf("\nBreadth First Traversal : ");
	m_breadth_traversal((void*)&start_vertex, m_graph_one, print_char);
	
	printf("\n");
	fclose(file);
	m_destroy_graph(m_graph_one);
	return 0;
}

void print_conneted(m_graph* pm_graph) //연결된 vertex를 출력하는 함수
{
	printf("The graph is :\n");
	memset(pm_graph->processed, 0, 20 * sizeof(int));
	for(j=0; j < pm_graph->count; j++)
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
			if ((pm_graph->compare(target_content, pm_graph->content[i]))==0)
				pm_graph->processed[i]=1;
		}

		printf("%c : ", *(char*)target_content);
		for (i = 0; i < pm_graph->count; i++)
		{
			if (!(pm_graph->compare(target_content, pm_graph->content[i])))
			{
				int k = 0;
				for (; k < pm_graph->count; k++)
				{
					if ((pm_graph->arc)[i][k])
						printf("%c ", *(char*)pm_graph->content[k]);
				}
			}
		}
		printf("\n");
	}
}