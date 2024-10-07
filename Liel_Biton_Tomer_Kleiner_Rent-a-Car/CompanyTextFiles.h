#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "RentalCompany.h"

int saveCompanyToTFile(const RentalCompany* pCompany, const char* fileName);
int writeVehicleArrayToTFile(FILE* fp, const RentalCompany* pCompany);
int writeCustomersArrayToTFile(FILE* fp, const RentalCompany* pCompany);
int writeRentalArrayToTFile(FILE* fp, const RentalCompany* pCompany);
int writeBranchListToTFile(FILE* fp, const RentalCompany* pCompany, NODE* head);
int writeVehicleToTFile(FILE* fp, Vehicle* pVehicle);
int writeVehicleUnionToTFile(FILE* fp, Vehicle* pVehicle);
int writeCustomerToTFile(FILE* fp, Customer* pCustomer);
int writeBranchToTFile(FILE* fp, Branch* pBranch);
int writeAddressToTFile(FILE* fp, Address* pAddress);
int writeRentalToTFile(FILE* fp, Rental* pRental);
int writeInsuranceToTFile(FILE* fp, Insurance* pInsurance);
int writeInvoiceToTFile(FILE* fp, Invoice* pInvoice);
int writeDateToTFile(FILE* fp, Date* pDate);

int initCompanyFromTFile(RentalCompany* pCompany, const char* fileName);
int readInsuranceFromTFile(FILE* fp, Insurance* pInsurance);
int readVehicleFromTFile(FILE* fp, Vehicle* pVehicle);
int readVehicleUnionFromTFile(FILE* fp, Vehicle* pVehicle);
int readInvoiceFromTFile(FILE* fp, Invoice* pInvoice);
int readCustomerFromTFile(FILE* fp, Customer* pCustomer);
int readBranchFromTFile(FILE* fp, Branch* pBranch);
int readAddressFromTFile(FILE* fp, Address* pAddress);
int readDateFromTFile(FILE* fp, Date* pDate);
int readRentalFromTFile(FILE* fp, Rental* pRental, Vehicle** vehicleArr, Customer* customerArr, int numCustomers, int numVehicles);

Vehicle** readVehicleArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numVehicles);
Customer* readCustomersArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numCustomers);
Rental* readRentalArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numRentals);
int readBranchListFromTFile(FILE* fp, RentalCompany* pCompany, int* numBranches);
