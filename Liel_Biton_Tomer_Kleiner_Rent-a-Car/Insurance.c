#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Macros.h"
#include "Insurance.h"
static unsigned int InsuranceSN_generator = START_SN_INSUR;

int createInsurance(Insurance* insurance)
{
	ERROR_ALOC_RETURN_NULL(insurance)
	insurance->type = getInsuranceType();
	insurance->costPerDay = costs[insurance->type];
	insurance->InsuranceSN = InsuranceSN_generator++;

	return 1;
}

unsigned int updateInsuranceGenerator(unsigned int num)
{
	InsuranceSN_generator = num;
	return ++InsuranceSN_generator;
}

unsigned int getCurrentInsuranceGenerator()
{
	return InsuranceSN_generator;
}

eType getInsuranceType() 
{
	int option;
	do {
		printf("\nPlease enter type of insurance\n");
		for (int i = 0; i < eNumTypes; i++)
			printf("%d for %s - costs %d$ per day\n", i, types[i], costs[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNumTypes);
	getchar();
	return (eType)option;
}

const char* GetTypeStr(int type)
{
	if (type < 0 || type >= eNumTypes)
		return NULL;
	return types[type];
}

void printInsurance(const Insurance* insurance)
{
	printf("Insurance number %d: %s\n", insurance->InsuranceSN, GetTypeStr((int)insurance->type));
}