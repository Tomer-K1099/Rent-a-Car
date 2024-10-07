#pragma once
#include <stdio.h>
#include "List.h"
#include "Rental.h"
#define DISCOUNT 20
typedef enum { eNoSort, eOdometer, eSerialNumber, eYear, eLicensePlate, eOpt } sortType;
static const char* sortName[eOpt] = { "Unsorted", "Odometer","Serial Number", "Manufacture Year","License Plate" };

typedef enum {eStartDate, eEndDate, eVehicle,eEnd, eNum} updateType;
static const char* updateName[eNum] = { "Start Date", "End Date","Vehicle","Finish Rental Today"};

typedef int (*CompareFunction)(const void*, const void*);
static const CompareFunction compareFunctions[] = { NULL, compareByOdometer, compareBySN, compareByYear, compareByLicensePlate};

typedef struct {
	char* companyName;
	LIST branch_list;
	Vehicle** vehicleArr;
	Customer* customerArr;
	Rental* rentalArr;
	int numVehicles;
	int numCustomers;
	int numRentals;
	int numBranches;
	sortType sortBy;
}RentalCompany;

int initCompany(RentalCompany* company);
int addBranch(RentalCompany* company);
int addVehicle(RentalCompany* company);
int addCustomer(RentalCompany* company);
int addRental(RentalCompany* company);
int chooseBranch(const RentalCompany* company);

Customer* findCustomerByID(Customer* customerArr, int numCustomers, char* ID);
Branch* findBranchByID(const RentalCompany* company, int branchID); 
void sortVehicles(RentalCompany* company);
void findVehicle(const RentalCompany* company);
sortType getSortType();
void askUserSearchParameter(Vehicle* toSearch, const RentalCompany* company);
void updateRental(RentalCompany* company); //Can update start date, end date or vehicle.
int updateRentalHelper(RentalCompany* company);
int chooseIndexFromRentalArray(RentalCompany* company);
Customer* chooseCustomerByID(const RentalCompany* company);
updateType getUpdateType();
Customer* SelectNewOrExistingCustomer(RentalCompany* company);
void RentalLotteryDiscount(RentalCompany* company); //The function randomly selects a rental that will receive a discount
void CalculateRevenueForSpecificYear(const RentalCompany* company);
int updateVehicleAvailability(RentalCompany* company);

void printAllVehicles(const RentalCompany* company);
void printCompany(const RentalCompany* company);
void printAllCustomers(const RentalCompany* company);
void printAllRentals(const RentalCompany* company);
void printAllBranches(const RentalCompany* company);
void printRentalsByBranch(const RentalCompany* company);

void freeVehicleArray(Vehicle** arr, int numVehicles);
void freeCustomerArray(Customer* arr, int numCustomers);
void freeCompany(RentalCompany* company);