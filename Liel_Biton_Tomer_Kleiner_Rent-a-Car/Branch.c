#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"
#include "Branch.h"
static int branchID_generator = START_ID_BRANCH;

int initBranch(Branch* branch) {

	ERROR_ALOC_RETURN_NULL(branch)
	initAddress(&branch->address);
	branch->branchID = branchID_generator++;
	return 1;
}

int getCurrentBranchGenerator()
{
	return branchID_generator;
}

int updateBranchGenerator(int num)
{
	branchID_generator = num;
	return ++branchID_generator;
}

void printBranch(const void* b) {
	Branch* branch = (Branch*)b;
	if (!branch)
		return;
	printf("Branch ID: %d\nBranch Address: ", branch->branchID);
	printAddress(&branch->address);
}

void freeBranch(Branch* branch)
{
	freeAddress(&branch->address);
}