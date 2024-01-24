#pragma warning(disable:4996)

#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _string_h
#define _string_h
#include <string.h>
#endif

#include "Ad_List.h"

#ifndef _repeat_variable
#define _repeat_variable1
int i = 0; //반복문을 위한 변수
int j = 0;
#endif

void print_conneted(l_graph* pl_graph); //연결된 vertex를 출력하는 함수

int main()
{
	l_graph* l_graph_one = l_create_graph(char_compare, char_free);

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
		add_result = l_add_vertex(l_graph_one, (void*)pchar_temp);
		ptr = strtok(NULL, " ");
		if (add_result == 0) free(pchar_temp); //만약 vertex가 이미 있으면 지금 free를 해주어야합니다.

		//한줄중 이후 문자를 처리, edge삽입
		char* pchar_temp2 = (char*)malloc(sizeof(char));
		to_vertex = *ptr;
		*pchar_temp2 = to_vertex;
		if (!pchar_temp2)
			return 0;
		add_result = l_add_vertex(l_graph_one, (void*)pchar_temp2);
		l_add_edge(l_graph_one, (void*)&from_vertex, (void*)&to_vertex);
		if (add_result == 0) free(pchar_temp2);

	}
	print_conneted(l_graph_one);

	printf("\nDepth First Traversal : ");
	l_depth_traversal((void*)&start_vertex, l_graph_one, print_char);

	printf("\nBreadth First Traversal : ");
	l_breadth_traversal((void*)&start_vertex, l_graph_one, print_char);

	printf("\n");
	fclose(file);
	l_destroy_graph(l_graph_one);
	return 0;
}

void print_conneted(l_graph* pl_graph) //연결된 vertex를 출력하는 함수
{
	printf("The graph is :\n");

	//processed를 0으로 초기화
	for (vertex_temp = pl_graph->first_vertex, i = 0; i < pl_graph->count; i++, vertex_temp = vertex_temp->next_vertex)
	{
		vertex_temp->processed = 0;
	}
	for (j = 0; j < pl_graph->count; j++)
	{
		void* target_content = NULL;
		vertex_temp = pl_graph->first_vertex;
		for (i = 0; i < pl_graph->count; i++,vertex_temp= vertex_temp->next_vertex)// target_content의 초기값을 처리안한 vertex 중  하나로 합니다.
		{
			if (vertex_temp->processed == 0)
			{
				target_content = vertex_temp->content;
				break;
			}
		}
		for (i = 1; i < pl_graph->count&&vertex_temp->next_vertex!=NULL; i++, vertex_temp = vertex_temp->next_vertex)// 처리를 안한 vertex중 가장 작은 알파벳vertex를 target_content가 가리키게 합니다.
		{
			if ((pl_graph->compare(target_content,vertex_temp->content ) > 0 && vertex_temp->processed == 0))
				target_content = vertex_temp->content ;
		}
		for (vertex_temp = pl_graph->first_vertex, i = 1; i < pl_graph->count; i++, vertex_temp = vertex_temp->next_vertex)// pogressed를 1로합니다.
		{
			if ((pl_graph->compare(target_content, vertex_temp->content) == 0))
			{
				vertex_temp->processed = 1;
				break;
			}
		}

		printf("%c : ", *(char*)target_content);
		for (arc_temp=vertex_temp->first_arc; arc_temp!=NULL; arc_temp=arc_temp->next_arc)
		{
			printf("%c ", *(char*)(((l_vertex*)arc_temp->destination)->content));
		}
		printf("\n");
	}
}