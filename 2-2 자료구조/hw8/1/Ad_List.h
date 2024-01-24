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

typedef struct L_arc {
	void* destination; //l_vertex* 대신 
	struct L_arc* next_arc;
}l_arc;

typedef struct L_vertex {
	void* content;
	struct L_vertex* next_vertex;
	l_arc* first_arc;
	int in_degree;
	int out_degree;
	int processed;
}l_vertex;

typedef struct L_graph {
	int count;
	l_vertex* first_vertex;
	int (*compare)(void* compare_one, void* compare_two); //content의 대소 비교 함수 포인터
	void (*free)(void* to_free_content); //동적할당된 content를 free하는 함수 포인터
}l_graph;

l_vertex* vertex_temp = NULL;
l_vertex* vertex_temp2 = NULL;
l_vertex* vertex_temp3 = NULL;
l_arc* arc_temp = NULL;
l_arc* arc_temp2 = NULL;

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



l_graph* l_create_graph(int (*compare_sort)(void* compare_one, void* compare_two), void (*content_free)(void* to_free_content))
{
	l_graph* pl_graph = (l_graph*)malloc(sizeof(l_graph));
	if (!pl_graph)
		return NULL;

	pl_graph->first_vertex = NULL;
	pl_graph->count = 0;
	pl_graph->compare = compare_sort;
	pl_graph->free = content_free;
		
	return pl_graph;
}

int l_add_vertex(l_graph* pl_graph, void* add_content)
{
	vertex_temp = pl_graph->first_vertex;
	for (i = 0; i < pl_graph->count; i++) 
	{
		if (!(pl_graph->compare(add_content,vertex_temp->content))) //이전 꼭지점중에 추가할려는 내용과 같은 것이 있으면 함수를 나갑니다.
			return 0;

		if (i < (pl_graph->count) - 1) //vertex_temp가 마지막 vertex에 멈추기 위하여 마지막은 예외 처리합니다.
			vertex_temp = vertex_temp->next_vertex;	
	}

	l_vertex* pl_vertex = (l_vertex*)malloc(sizeof(l_vertex));
	if (!pl_vertex)
		return 0;

	pl_vertex->content = add_content;
	pl_vertex->next_vertex = NULL;
	pl_vertex->first_arc = NULL;
	pl_vertex->in_degree = 0;
	pl_vertex->out_degree = 0;
	pl_vertex->processed = 0;

	if (vertex_temp == NULL)
	{
		pl_graph->first_vertex = pl_vertex;
	}
	else
	{
		vertex_temp->next_vertex = pl_vertex;
	}
	(pl_graph->count)++;

	return 1;
}


void l_delete_edge(l_graph* pl_graph, void* from_vertex, void* to_vertex)
{
	//from_vertex를 vertex_temp가 가리키게 합니다.
	for (i = 0, vertex_temp = pl_graph->first_vertex; i < pl_graph->count&&pl_graph->compare(from_vertex, vertex_temp->content); i++, vertex_temp = vertex_temp->next_vertex);

	arc_temp = vertex_temp->first_arc;
	if (pl_graph->compare((((l_vertex*)arc_temp)->content), to_vertex) == 0)  //첫 arc 삭제인 경우
	{
		arc_temp2 = arc_temp;
		vertex_temp->first_arc = NULL;
		free(arc_temp2);
		vertex_temp->out_degree = 0;
	}
	else
	{
		while (pl_graph->compare((((l_vertex*)(arc_temp->destination))->content), to_vertex) != 0) //arc_temp가 지워야할 arc의 전 arc를 가리키게 합니다.
		{
			arc_temp = arc_temp->next_arc;
		}
		arc_temp2 = arc_temp->next_arc;
		arc_temp->next_arc = arc_temp2->next_arc;
		free(arc_temp2);
		(vertex_temp->out_degree)--;
	}

	//to_vertex를 vertex_temp가 가리키게 합니다.
	for (i = 0, vertex_temp = pl_graph->first_vertex; i < pl_graph->count && pl_graph->compare(from_vertex, vertex_temp->content); i++, vertex_temp = vertex_temp->next_vertex);
	(vertex_temp->in_degree)--;
}

int l_delete_vertex(l_graph* pl_graph, void* delete_content)
{
	//삭제하려는 vertex를 vertex_temp3가 가리키게 합니다.
	for (j=0,vertex_temp3 = pl_graph->first_vertex; j < pl_graph->count && pl_graph->compare(delete_content, vertex_temp3->content);j++, vertex_temp3 = vertex_temp->next_vertex);
	
	if (vertex_temp3 == NULL) //삭제하려는 vertex가 없는 경우
		return 0;

	//삭제할려는 vertex의 outdegree를 없앴니다.
	for (j = 0, vertex_temp2 = pl_graph->first_vertex; j < pl_graph->count; j++, vertex_temp2 = vertex_temp2->next_vertex);
	{
		for (arc_temp= vertex_temp2->first_arc; arc_temp != NULL; arc_temp = arc_temp->next_arc)
		{
			if (pl_graph->compare((((l_vertex*)(arc_temp->destination))->content), delete_content))
				l_delete_edge(pl_graph, vertex_temp2->content, delete_content);
		}
	}

	for (arc_temp = vertex_temp3->first_arc; arc_temp != NULL; arc_temp = arc_temp->next_arc)
	{
		l_delete_edge(pl_graph, delete_content,((l_vertex*)(arc_temp->destination))->content);
	}
	pl_graph->free(vertex_temp3->content);
	free(vertex_temp3);
	return 1;
}

void l_add_edge(l_graph* pl_graph, void* from_vertex, void* to_vertex)
{
	
	l_arc* pl_arc = (l_arc*)malloc(sizeof(l_arc)); //arc생성
	if (!pl_arc)
		return;
	//to_vertex를 vertex_temp가 가리키게 합니다.
	for (i = 0, vertex_temp = pl_graph->first_vertex; i < pl_graph->count && pl_graph->compare(to_vertex, vertex_temp->content); i++, vertex_temp = vertex_temp->next_vertex);

	pl_arc->destination = (void*)vertex_temp;
	pl_arc->next_arc = NULL;

	//from_vertex를 vertex_temp가 가리키게 합니다.
	for (i = 0, vertex_temp = pl_graph->first_vertex; i < pl_graph->count && pl_graph->compare(from_vertex, vertex_temp->content); i++, vertex_temp = vertex_temp->next_vertex);

	if (vertex_temp->first_arc == NULL) vertex_temp->first_arc = pl_arc; //첫 arc일때	
	else
	{
		arc_temp = vertex_temp->first_arc;
		while ((arc_temp->next_arc)!= NULL)
		{
			arc_temp = arc_temp->next_arc;
		}
		arc_temp->next_arc = pl_arc;
	}
	(vertex_temp->out_degree)++;
	//to_vertex를 vertex_temp가 가리키게 합니다.
	for (i = 0, vertex_temp = pl_graph->first_vertex; i < pl_graph->count && pl_graph->compare(from_vertex, vertex_temp->content); i++, vertex_temp = vertex_temp->next_vertex);
	(vertex_temp->in_degree)++;
}

void l_destroy_graph(l_graph* pl_graph)
{
	while (pl_graph->count)
	{
		l_delete_vertex(pl_graph, pl_graph->first_vertex->content);
	}
	free(pl_graph);
}

void _l_depth_traversal(l_graph* pl_graph, l_vertex* progressing_vertex, void (*progress)(void* content))
{
	progress(progressing_vertex->content);
	(progressing_vertex->processed)= 1;

	l_arc* progressing_arc = progressing_vertex->first_arc; //재귀함수이므로 새 포인터를 설정합니다.
	for (; progressing_arc != NULL; progressing_arc = progressing_arc->next_arc)
	{
		if (((l_vertex*)(progressing_arc->destination))->processed == 0)
		{
			l_vertex* next_progressing_vertex = (l_vertex*)(progressing_arc->destination);
			_l_depth_traversal(pl_graph, next_progressing_vertex, progress);
		}
	}

}

void l_depth_traversal(void* start_vertex, l_graph* pl_graph, void (*progress)(void* content))
{
	if (pl_graph->count == 0) return; //빈 그래프일때

	//vertex_temp가 start_vertex를 가리키게 합니다.
	for (vertex_temp = pl_graph->first_vertex; vertex_temp != NULL && pl_graph->compare(start_vertex, (vertex_temp->content)); vertex_temp = vertex_temp->next_vertex);
	
	if (vertex_temp == NULL)  //start_vertex를 찾지 못했을 때
		return;

	//processed를 0으로 초기화
	for (vertex_temp2 = pl_graph->first_vertex,i = 0; i < pl_graph->count; i++, vertex_temp2 = vertex_temp2->next_vertex)
	{
		vertex_temp2->processed = 0;
	}

	_l_depth_traversal(pl_graph, vertex_temp, progress);
}

void _l_breadth_traversal(l_graph* pl_graph, l_vertex* progressing_vertex, void (*progress)(void* content), queuehead* vertex_queue)
{
	progress(progressing_vertex->content);
	(progressing_vertex->processed) = 1;

	l_arc* progressing_arc = progressing_vertex->first_arc; //재귀함수이므로 새 포인터를 설정합니다.
	for (; progressing_arc != NULL; progressing_arc = progressing_arc->next_arc)
	{
		enqueue(vertex_queue, progressing_arc->destination);
	}
	while (isemptyqueue(vertex_queue) == 0)
	{
		l_vertex* content_temp = (l_vertex*)dequeue(vertex_queue);
		
		if (content_temp->processed == 0) _l_breadth_traversal(pl_graph, content_temp, progress, vertex_queue);
	}
}

void l_breadth_traversal(void* start_vertex, l_graph* pl_graph, void (*progress)(void* content))
{
	if (pl_graph->count == 0) return; //빈 그래프일때

	//vertex_temp가 start_vertex를 가리키게 합니다.
	for (vertex_temp = pl_graph->first_vertex; vertex_temp != NULL && pl_graph->compare(start_vertex, (vertex_temp->content)); vertex_temp = vertex_temp->next_vertex);

	if (vertex_temp == NULL)  //start_vertex를 찾지 못했을 때
		return;

	//processed를 0으로 초기화
	for (vertex_temp2 = pl_graph->first_vertex, i = 0; i < pl_graph->count; i++, vertex_temp2 = vertex_temp2->next_vertex)
	{
		vertex_temp2->processed = 0;
	}

	queuehead* queue_one = createqueue();
	_l_breadth_traversal(pl_graph, vertex_temp, progress, queue_one);
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