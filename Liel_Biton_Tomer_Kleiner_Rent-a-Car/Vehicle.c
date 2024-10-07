#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Vehicle.h"
#include "General.h"

void initPremium(Vehicle** vehicleArr, Vehicle* premium, int vehicleCount)
{
	initVehicle(vehicleArr, premium, vehicleCount);
	premium->print = printPremium;
	premium->costPerDay = DAY_PRICE * PREM_multiplier;
	premium->categoryType = 0;
	if (premium->gearBox == 1) //A manual vehicle cannot be electric.
		premium->category.premium.isElectric = getYesNoAnswer("\nEnter 1 for electric vehicle\nEnter 0 if other");
	else
		premium->category.premium.isElectric = 0;
	premium->category.premium.feature = getPremiumFeature();
	premium->category.premium.brand = getVehicleBrand(PremiumBrand, eNumPremiumBrands);
}

void initStandard(Vehicle** vehicleArr, Vehicle* standard, int vehicleCount)
{
	initVehicle(vehicleArr, standard, vehicleCount);
	standard->print = printStandard;
	standard->categoryType = 1;
	standard->costPerDay = DAY_PRICE * STAN_multiplier;
	standard->category.standard.cargoCapacity = getDoubleNum("Enter cargo capacity: ");
	standard->category.standard.brand = getVehicleBrand(StandardBrand, eNumStandardBrands);
}

void initCompact(Vehicle** vehicleArr, Vehicle* compact, int vehicleCount)
{
	initVehicle(vehicleArr, compact, vehicleCount);
	compact->print = printCompact;
	compact->categoryType = 2;
	compact->costPerDay = DAY_PRICE * COMP_multiplier;
	compact->category.compact.fuelEfficiency = getDoubleNum("Enter the fuel efficiency: ");
	compact->category.compact.brand = getVehicleBrand(CompactBrand, eNumCompactBrands);
}

void initVehicle(Vehicle** vehicleArr, Vehicle* vehicle, int vehicleCount)
{
	int ok;
	vehicle->vehicleSN = getVehicleSN(vehicleArr, vehicleCount);
	vehicle->year = getVehicleYear();
	vehicle->numSeats = getNumSeats();
	vehicle->gearBox = getYesNoAnswer("\nEnter 1 for automatic transmission\nEnter 0 for manual transmission");
	vehicle->isTaken = 0;
	vehicle->odometer = 0;
	do {
		getLicensePlate(vehicle);
		ok = checkUniquePlate(vehicle->licensePlate, vehicleArr, vehicleCount);
		if (!ok)
			printf("There is a vehicle with this license plate. Try again\n");
	} while (!ok);	
}

eFeatures getPremiumFeature()
{
	int option;
	printf("\n");
	do {
		printf("Please choose one of the following features\n");
		for (int i = 0; i < eNofOpt; i++)
			printf("%d for %s\n", i, Features[i]);
		while (!scanf("%d", &option));
	} while (option < 0 || option >= eNofOpt);
	getchar();
	return (eFeatures)option;
}

int getVehicleBrand(char** arrName, int numOfOpt)
{
	int option;
	printf("\n");
	do {
		printf("Please choose one of the following brands\n");
		for (int i = 0; i < numOfOpt; i++)
			printf("%d for %s\n", i, arrName[i]);
		while (!scanf("%d", &option));
	} while (option < 0 || option >= numOfOpt);
	getchar();
	return option;
}

void getLicensePlate(Vehicle* vehicle)
{
	char plate[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter License Plate - max %d chars\n", MAX_PLATE);
		myGets(plate, MAX_STR_LEN, stdin);
		if (strlen(plate) > MAX_PLATE)
		{
			printf("License plate should be maximum %d chars\n", MAX_PLATE);
			ok = 0;
		}
	} while (!ok);
	strcpy(vehicle->licensePlate, plate);
}

int getNumSeats()
{
	int num;
	do {
		printf("Enter number of seats - min %d max %d\n", MIN_SEATS, MAX_SEATS);
		while (!scanf("%d", &num));
	} while (num < MIN_SEATS || num > MAX_SEATS);
	return num;
}

int getVehicleYear()
{
	int num;
	do {
		printf("Enter vehicle year of manufacture (between %d - %d)\n", MIN_MAN_YEAR, MAX_MAN_YEAR);
		while (!scanf("%d", &num));
		if (num < MIN_MAN_YEAR)
			printf("The year is under the minimum. Try again.\n");
		if (num > MAX_MAN_YEAR)
			printf("The year is over the maximum. Try again.\n");
	} while (num < MIN_MAN_YEAR || num > MAX_MAN_YEAR);
	return num;
}

int getVehicleSN(Vehicle** vehicleArr, int vehicleCount)
{
	int num,ok;
	do {
		printf("Enter vehicle serial number\n");
		while (!scanf("%d", &num));
		ok = checkUniqueSN(num, vehicleArr, vehicleCount);
		if (!ok)
			printf("This serial number already exist. Try again.\n");
	} while (!ok);
	return num;
}

void updateOdometer(Vehicle* vehicle, int totalDays, int kmPerDay)
{
	vehicle->odometer += (totalDays * kmPerDay);
}

int rentVehicle(Vehicle* vehicle)
{
	vehicle->isTaken = 1;
	return 1;
}

int checkUniqueSN(int SN, Vehicle** vehicleArr, int vehicleCount)
{
	for (int i = 0; i < vehicleCount; i++)
	{
		if (vehicleArr[i]->vehicleSN == SN)
			return 0;
	}
	return 1;
}

int checkUniquePlate(char* plate, Vehicle** vehicleArr, int vehicleCount)
{
	for (int i = 0; i < vehicleCount; i++)
	{
		if (strcmp(vehicleArr[i]->licensePlate, plate) == 0)
			return 0;
	}
	return 1;
}

int compareByOdometer(const void* v1, const void* v2)
{
	Vehicle* e1 = *((Vehicle**)v1);
	Vehicle* e2 = *((Vehicle**)v2);
	return (int)(e1->odometer - e2->odometer);
}

int compareByYear(const void* v1, const void* v2)
{
	Vehicle* e1 = *((Vehicle**)v1);
	Vehicle* e2 = *((Vehicle**)v2);
	return e1->year - e2->year;
}

int compareBySN(const void* v1, const void* v2)
{
	Vehicle* e1 = *((Vehicle**)v1);
	Vehicle* e2 = *((Vehicle**)v2);
	return e1->vehicleSN - e2->vehicleSN;
}

int compareByLicensePlate(const void* v1, const void* v2)
{
	Vehicle* e1 = *((Vehicle**)v1);
	Vehicle* e2 = *((Vehicle**)v2);
	return strcmp(e1->licensePlate, e2->licensePlate);
}

void printVehicle(const Vehicle* vehicle)
{
	printf("- - - - - - - - - - - - - - \n");
	printf("%s Vehicle number: %d\nLicense plate: %s\n", Categories[vehicle->categoryType], vehicle->vehicleSN, vehicle->licensePlate);
	printf("Has %s gearbox\n", vehicle->gearBox == 1 ? "automatic" : "manual");
	printf("Number of seats: %d\tYear: %d\tOdometer: %.3f\n", vehicle->numSeats, vehicle->year, vehicle->odometer);	
}

void printVehicleV(void* vehicle)
{
	Vehicle* pVehicle = *(Vehicle**)vehicle;
	pVehicle->print(pVehicle);
}

void printPremium(const Vehicle* premium)
{
	printVehicle(premium);
	printf("Brand: %s\t", PremiumBrand[premium->category.premium.brand]);
	printf("Feature: %s\n", Features[premium->category.premium.feature]);
	if (premium->category.premium.isElectric)
		printf("Vehicle is electric.\n");
}
void printStandard(const Vehicle* standard)
{
	printVehicle(standard);
	printf("Brand: %s\t", StandardBrand[standard->category.standard.brand]);
	printf("With %.3f cargo capacity\n", standard->category.standard.cargoCapacity);

}
void printCompact(const Vehicle* compact) 
{
	printVehicle(compact);
	printf("Brand: %s\t", CompactBrand[compact->category.compact.brand]);
	printf("With %.3f fuel Efficiency\n", compact->category.compact.fuelEfficiency);
}

void freeVehiclePtr(void* pVehiclePtr)
{
	Vehicle* temp = *(Vehicle**)pVehiclePtr;
	free(temp);
}