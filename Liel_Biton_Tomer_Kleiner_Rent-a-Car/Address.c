#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Address.h"

void initAddress(Address* address) { 
	int choice;
	do {
		printf("\nSelect country:");
		for (int i = 0; i < eNumCountries; i++)
			printf("\n[%d] for %s", i, Countries[i]);
		printf("\n");
	} while (!scanf("%d", &choice) || choice >= eNumCountries || choice < 0);
	address->country = choice;
	address->city = getStrExactName("\nEnter city name:");
	if (!address->city) return;
	address->street = getStrExactName("\nEnter street name:");
	if (!address->street) return;
	do {
		printf("\nEnter number:\n");
	} while (!scanf("%d", &address->number));

}

void printAddress(const Address* address) {

	printf("%s %d, %s, %s\n", address->street, address->number, address->city, Countries[address->country]);

}

void freeAddress(Address* address)
{
	free(address->city);
	free(address->street);
}