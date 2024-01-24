#ifndef _stdio_h
#define _stdio_h
#include <stdio.h>
#endif

#ifndef _stdlib_h
#define _stdlib_h
#include <stdlib.h>
#endif

#ifndef _element
#define _element
typedef void* element;
#endif

#include"Tree_Queue.h"

#define PRE_ORDER 1
#define INFIX_ORDER 2
#define POST_ORDER 3
#define BREATH_FIRST 4

typedef struct Tree_Node {
	struct Tree_Node* left;
	element content;
	struct Tree_Node* right;
}tree_node;

typedef struct Tree_Head {
	int count;
	tree_node* root;
}tree_head;


tree_head* bt_create()
{
	tree_head* ptree_head = (tree_head*)malloc(sizeof(tree_head));
	if (!ptree_head)
		return NULL;
	ptree_head->count = 0;
	ptree_head->root = NULL;

	return ptree_head;
}

int bt_count(tree_head* ptree_head)
{
	return ptree_head->count;
}

int bt_empty(tree_head* ptree_head)
{
	if (ptree_head->count == 0) return 1;
	else return 0;
}

tree_node* node_address(tree_head* ptree_head, int node_number)//node_number번째 노드의 주소를 리턴
{
	if (node_number == 0) //root일 경우
	{
		return ptree_head->root;
	}
	if (node_number % 2 == 1)
	{
		return node_address(ptree_head, (node_number - 1) / 2)->left;
	}
	else
	{
		return node_address(ptree_head, (node_number - 2) / 2)->right;
	}
}

void bt_insert(tree_head* ptree_head, element pcontent)
{
	tree_node* ptree_node = (tree_node*)malloc(sizeof(tree_node));
	if (!ptree_node)
		return;
	ptree_node->content = pcontent;
	ptree_node->left = NULL;
	ptree_node->right = NULL;

	if (ptree_head->count == 0) //빈 트리
	{
		ptree_head->root = ptree_node;
	}
	else //빈 트리가 아닐 경우
	{ //이전 노트들에서 왼쪽 오른쪽순으로 차례차례 채웁니다.
		if ((ptree_head->count) % 2 == 1)
		{ //어떤 노드의 왼쪽에 붙는 경우
			node_address(ptree_head ,((ptree_head->count) - 1) / 2)->left = ptree_node;
		}
		else
		{ //어떤 노드의 오른쪽에 붙는 경우
			node_address(ptree_head, ((ptree_head->count) - 2) / 2)->right = ptree_node;
		}
	}
	(ptree_head->count)++;
}

void bt_traverse(tree_node* root, void (*process)(void*), int order)
{
	if (root)
	{
		switch (order)
		{
		case PRE_ORDER:
		{
			process(root->content);
			bt_traverse(root->left, process, 1);
			bt_traverse(root->right, process, 1);
			break;
		}
		case INFIX_ORDER:
		{
			bt_traverse(root->left, process, 2);
			process(root->content);
			bt_traverse(root->right, process, 2);
			break;
		}
		case POST_ORDER:
		{
			bt_traverse(root->left, process, 3);
			bt_traverse(root->right, process, 3);
			process(root->content);
			break;
		}
		case BREATH_FIRST:
		{
			queuehead* queue_one = createqueue();
			enqueue(queue_one, (void*)root);
			tree_node* temp_node = NULL;
			while (!isemptyqueue(queue_one))
			{
				temp_node = (tree_node*)dequeue(queue_one);
				process(temp_node->content);
				if (temp_node->left != NULL)
				{
					enqueue(queue_one, (void*)temp_node->left);
				}
				if (temp_node->right != NULL)
				{
					enqueue(queue_one, (void*)temp_node->right);
				}
			}
			destroyqueue(queue_one);
			break;
		}
		default:
			break;
		}
	}
}

void _destroy(tree_node* ptree_node)
{
	if (ptree_node == NULL)
		return;

	_destroy(ptree_node->left);
	_destroy(ptree_node->right);
	free(ptree_node);
}

void bt_destroy(tree_head* ptree_head)
{
	_destroy(ptree_head->root); //모든 노드를 제거합니다.
	free(ptree_head);
}

void print_char(void* pchar)
{
	printf("%c", *(char*)pchar);
}

/*
retrieve와delete는 과제에서 쓰이지 않으며,
content의 대소의 의한 순서가 없는 complete binary tree특성상 의미가 없고,
문자열에서 같은 문자가 중복해서 쓰였을때 같은 경우도 있어서 아예 안 만들었습니다.
*/