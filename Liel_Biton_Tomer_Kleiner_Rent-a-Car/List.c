/**************/
/*   list.c   */
/**************/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if (pList == NULL)
		return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, void* val)
{
	NODE* tmp;

	if (!pNode)
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp != NULL) {
		tmp->key = val;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode, void(*freeKey)(void*))
{
	NODE* tmp;
	if (!pNode)
		return False;
	tmp = pNode->next;
	if (!tmp)
		return False;

	pNode->next = tmp->next;
	if (freeKey)
		freeKey(tmp->key);
	free(tmp);
	return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE* L_find(const NODE* pNode, void* val, int (*compare)(const void*, const void*))
{
	const NODE* tmp = NULL;

	while (pNode != NULL)
	{
		if (!compare(pNode->key, val)) {
			tmp = pNode;
			break;
		}
		pNode = pNode->next;
	}

	return tmp;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList, void (*freeKey)(void*))
{
	NODE* tmp;
	if (!pList)
		return False;

	for (tmp = &(pList->head); L_delete(tmp, freeKey); tmp = tmp->next);
	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList, void (*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList)
		return False;

	tmp = pList->head.next;
	while (tmp != NULL)
	{
		print(tmp->key);
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}
