#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Rental.h"
static int rentalSN_generator = START_SN_RENT;

int initRental(Rental* rental, Customer* customer, const Vehicle** vehicleArr, Rental* rentalArr,int numRentals,int numVehicles, int branchID)
{
	if (getCurrentRentalGenerator() > MAX_SN_RENT)
	{
		printf("Maximum rentals limit.\n");
		return 0;
	}
	rental->rentalSN = rentalSN_generator++;
	printf("Enter Start Date: ");
	getCorrectDate(&rental->startDate);
	int ok = 0;
	do {
		printf("Enter End Date: ");
		getCorrectDate(&rental->endDate);
		ok = checkRentDates(rental->startDate, rental->endDate);
		if (!ok)
			printf("End date is earlier than start date. Try again.\n");
	} while (!ok);
	rental->customer = *customer;
	rental->vehicle = getVehicleInRental(vehicleArr, rentalArr, numRentals, numVehicles, numRentals);
	rental->branchID = branchID;
	createInsurance(&rental->insurance);
	rental->totalCost = 0;
	createInvoice(&rental->invoice, rental->totalCost, rental->rentalSN);
	return 1;
}

Vehicle* getVehicleInRental(const Vehicle** vehicleArr, Rental* rentalArr, int numRentals, int numVehicles, int rentalIndex)
{
	Vehicle* newVehicle;
	int ok = 0;
	if (!numVehicles)
	{
		printf("No vehicles to choose from. try adding a vehicle first.");
		return NULL;
	}
	do {
		printAvailableVehicles(vehicleArr, rentalArr, numRentals, numVehicles, &rentalArr[rentalIndex]);
		ok = getIntegerNum("\nChoose the serial number of the vehicle: ");
		newVehicle = findVehicleBySN(vehicleArr, numVehicles, ok);
		if (!newVehicle)
			printf("No vehicle with this Serial Number. Try again!\n");
	} while (!newVehicle);
	return newVehicle;
}

Vehicle* findVehicleBySN(Vehicle** vehicleArr, int numVehicles, int SN)
{
	for (int i = 0; i < numVehicles; i++)
	{
		if (vehicleArr[i]->vehicleSN == SN)
			return vehicleArr[i];
	}
	return NULL;
}

int updateRentalGenerator(int num)
{
	rentalSN_generator = num;
	return ++rentalSN_generator;
}

int getCurrentRentalGenerator()
{
	return rentalSN_generator;
}

float calculateTotalCost(Rental* rental)
{
	float num = 0.0;
	int days = calculateDaysOfRental(rental->startDate, rental->endDate);
	num = num + (days * (rental->insurance.costPerDay + rental->vehicle->costPerDay));
	return num;
}

int endRental(Rental* rental)
{
	int ok = 0;
	if (isDatePassedOrToday(&rental->startDate) == 1)
		getTodaysDate(&rental->endDate);
	else
	{
		printf("End date is earlier than start date. Try again.\n");
		return 0;
	}
	rental->vehicle->isTaken = 0;
	updateOdometer(rental->vehicle, calculateDaysOfRental(rental->startDate, rental->endDate), AVG_KM);
	printf("Rental was successfully completed\n");
	printInvoice(&rental->invoice);
	printf("You deserve a %.2f dollar credit", rental->totalCost - calculateTotalCost(rental));
	return 1;
}

int isVehicleAvailableInDates(const Rental* rentalArr, int numRentals, const Vehicle* vehicle, const Date* start, const Date* end) // Return 1 if vehicle is available
{
	for (int i = 0; i < numRentals; i++)
	{
		if (rentalArr[i].vehicle->vehicleSN == vehicle->vehicleSN)
		{
			if (!dateRangesDoNotCollide(&rentalArr[i].startDate, &rentalArr[i].endDate, start, end))
				return 0; // the vehicle is unAvailable in the dates
		}
	}
	return 1;
}

void printAvailableVehicles(const Vehicle** vehicleArr,const Rental* rentalArr, int numRentals, int numVehicles, const Rental* rental) // Print available vehicles considering the NEW rental chosen dates
{
	printf("\n--- Available Vehicles ---\n");
	for (int i = 0; i < numVehicles; i++)
	{
		if (vehicleArr[i]->isTaken == 1)
		{
			if (isVehicleAvailableInDates(rentalArr, numRentals, vehicleArr[i], &rental->startDate, &rental->endDate))
				vehicleArr[i]->print(vehicleArr[i]);
		}
		else //Vehicle isnt taken.
			vehicleArr[i]->print(vehicleArr[i]);
	}
}

void printRental(const Rental* rental)
{
	printf("\033[1;32mRental serial number: %d\033[0m\n", rental->rentalSN);
	printCustomer(&rental->customer);
	printf("Rent From Date");
	printDate(&rental->startDate);
	printf("Until Date");
	printDate(&rental->endDate);
	printf("\nPick up vehicle number %d from branch number %d\n", rental->vehicle->vehicleSN, rental->branchID);
	printInsurance(&rental->insurance);
	printInvoice(&rental->invoice);
}

void freeRental(Rental* rental)
{
	free(rental->vehicle);
	freeCustomer(&rental->customer);
}