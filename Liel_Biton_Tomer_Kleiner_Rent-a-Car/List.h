#pragma once
/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/

#include "def.h"

/*** Definitions ***/

// List
typedef struct
{
	NODE head;
}LIST;


/*** Function prototypes ***/

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, void* val);	// add new node after *pNode

BOOL L_delete(NODE* pNode, void(*freeKey)(void*));					// erase node after *pNode

const NODE* L_find(const NODE* pNode, void* val, int (*compare)(const void*, const void*));	// return a pointer to the node

BOOL L_free(LIST* pList, void(*freeKey)(void*));					// free list memory

int L_print(const LIST* pList, void (*print)(const void*));				// print the list content

