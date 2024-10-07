#pragma once
#include <stdio.h>
#include "Address.h"
#define START_ID_BRANCH 0

typedef struct{

	Address address;
	int branchID;
}Branch;

int initBranch(Branch* branch);
int updateBranchGenerator(int num);
int getCurrentBranchGenerator();
void printBranch(const void* branch);
void freeBranch(Branch* branch);