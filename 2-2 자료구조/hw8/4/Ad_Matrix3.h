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
#define _repeat_variable
int i = 0; //반복문을 위한 변수
int j = 0;
#endif

#include "Queue.h"

typedef struct M_graph {
	void* content[20];
	int processed[20];
	int arc[20][20];
	int count;
	int (*compare)(void* compare_one, void* compare_two); //content의 대소 비교 함수 포인터
	void (*free)(void* to_free_content); //동적할당된 content를 free하는 함수 포인터
}m_graph;

int char_compare(void* compare_one, void* compare_two)
{
	return (int)(*(char*)compare_one - *(char*)compare_two);
}

void char_free(void* to_free_content)
{
	free((char*)to_free_content);
}

void print_char(void* content)
{
	printf("%c ", *(char*)content);
}


m_graph* m_create_graph(int (*compare_sort)(void* compare_one, void* compare_two), void (*content_free)(void* to_free_content))
{
	m_graph* pm_graph = (m_graph*)malloc(sizeof(m_graph));
	if (!pm_graph)
		return NULL;


	memset(pm_graph->processed, 0, 20 * sizeof(int));
	memset(pm_graph->arc, 0, 400 * sizeof(int));
	pm_graph->count = 0;
	pm_graph->compare = compare_sort;
	pm_graph->free = content_free;

	return pm_graph;
}

int m_add_vertex(m_graph* pm_graph, void* add_content)
{
	for (i = 0; i < pm_graph->count; i++) //이전 꼭지점중에 추가할려는 내용과 같은 것이 있으면 함수를 나갑니다.
	{
		if (!(pm_graph->compare(add_content, pm_graph->content[i])))
			return 0;
	}

	pm_graph->content[(pm_graph->count)++] = add_content;
	return 1;

}

void m_add_edge(m_graph* pm_graph, void* from_vertex, void* to_vertex,int cost)
{
	for (i = 0; i < pm_graph->count; i++)
	{
		if (!(pm_graph->compare(from_vertex, pm_graph->content[i]))) break;
	}
	for (j = 0; j < pm_graph->count; j++)
	{
		if (!(pm_graph->compare(to_vertex, pm_graph->content[j]))) break;
	}
	if (i == pm_graph->count || j == pm_graph->count) return; //입력된 내용의 vertex가 없을때
	((pm_graph->arc)[i][j])= ((pm_graph->arc)[i][j])+cost;
}

void m_delete_edge(m_graph* pm_graph, void* from_vertex, void* to_vertex)
{
	for (i = 0; i < pm_graph->count; i++)
	{
		if (!(pm_graph->compare(from_vertex, pm_graph->content[i]))) break;
	}
	for (j = 0; j < pm_graph->count; j++)
	{
		if (!(pm_graph->compare(to_vertex, pm_graph->content[j]))) break;
	}
	if (i == pm_graph->count || j == pm_graph->count) return; //입력된 내용의 vertex가 없을때
	if ((pm_graph->arc)[i][j])
		((pm_graph->arc)[i][j])=0;
}

void m_destroy_graph(m_graph* pm_graph)
{
	for (i = 0; i < pm_graph->count; i++) //content에 담긴 주소를 직접 찾아가 실제 데이터가 저장된 공간을 free합니다.
	{
		pm_graph->free((pm_graph->content)[i]);
	}
	free(pm_graph);
}

void _m_depth_traversal(m_graph* pm_graph, int vertex_number, void* vertex_content, void (*progress)(void* content))
{
	progress(vertex_content);
	(pm_graph->processed)[vertex_number] = 1;
	int k = 0; //전역변수는 재귀함수에 적합하지 않으므로 새 변수를 선언합니다.
	for (; k < 20; k++)
	{
		if ((pm_graph->arc)[vertex_number][k] && (pm_graph->processed)[k] == 0)
			_m_depth_traversal(pm_graph, k, (pm_graph->content)[k], progress);
	}
}

void m_depth_traversal(void* start_vertex, m_graph* pm_graph, void (*progress)(void* content))
{
	if (pm_graph->count == 0) return; //빈 그래프일때

	for (i = 0; i < 20; i++) //지금 vertex가 몇 번째 vertex인지 알기 위하여
	{
		if (!(pm_graph->compare(start_vertex, pm_graph->content[i])))
			break;
	}

	memset(pm_graph->processed, 0, 20 * sizeof(int));
	_m_depth_traversal(pm_graph, i, pm_graph->content[i], progress);
}

void _m_breadth_traversal(m_graph* pm_graph, int vertex_number, void* vertex_content, void (*progress)(void* content), queuehead* vertex_queue)
{
	progress(vertex_content);
	(pm_graph->processed)[vertex_number] = 1;
	for (i = 0; i < 20; i++)
	{
		if ((pm_graph->arc)[vertex_number][i] && (pm_graph->processed)[i] == 0)
			enqueue(vertex_queue, (pm_graph->content)[i]);
	}
	while (isemptyqueue(vertex_queue) == 0)
	{
		void* content_temp = dequeue(vertex_queue);
		for (i = 0; i < 20; i++) //지금 꺼낸 vertex가 몇 번째 vertex인지 알기 위하여
		{
			if (!(pm_graph->compare(content_temp, pm_graph->content[i])))
				break;
		}
		if ((pm_graph->processed)[i] == 0) _m_breadth_traversal(pm_graph, i, content_temp, progress, vertex_queue);
	}
}

void m_breadth_traversal(void* start_vertex, m_graph* pm_graph, void (*progress)(void* content))
{
	if (pm_graph->count == 0) return; //빈 그래프일때

	for (i = 0; i < 20; i++) //지금 vertex가 몇 번째 vertex인지 알기 위하여
	{
		if (!(pm_graph->compare(start_vertex, pm_graph->content[i])))
			break;
	}

	memset(pm_graph->processed, 0, 20 * sizeof(int));
	queuehead* queue_one = createqueue();
	_m_breadth_traversal(pm_graph, i, pm_graph->content[i], progress, queue_one);
	destroyqueue(queue_one);
}




/*
	m_create_graph
	m_add_vertex
	m_add_edge
	m_delete_edge
	m_traversal
	m_destroy_graph
*/