#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "RentalCompany.h"

int saveCompanyToBFile(const RentalCompany* pCompany, const char* fileName);
int writeVehicleArrayToBFile(FILE* fp, const RentalCompany* pCompany);
int writeCustomersArrayToBFile(FILE* fp, const RentalCompany* pCompany);
int writeRentalArrayToBFile(FILE* fp, const RentalCompany* pCompany);
int writeBranchListToBFile(FILE* fp, const RentalCompany* pCompany, NODE* head);
int writeVehicleToBFile(FILE* fp, Vehicle* pVehicle);
int writeCustomerToBFile(FILE* fp, Customer* pCustomer);
int writeBranchToBFile(FILE* fp, Branch* pBranch);
int writeAddressToBFile(FILE* fp, Address* pAddress);
int writeRentalToBFile(FILE* fp, Rental* pRental);
int writeCompressedInsuranceToBFile(FILE* fp, Insurance* pInsurance);

int initCompanyFromBFile(RentalCompany* pCompany, const char* fileName);
int readCompressedInsuranceFromBFile(FILE* fp, Insurance* pInsurance);
int readVehicleFromBFile(FILE* fp, Vehicle* pVehicle);
int readCustomerFromBFile(FILE* fp, Customer* pCustomer);
int readBranchFromBFile(FILE* fp, Branch* pBranch);
int readAddressFromBFile(FILE* fp, Address* pAddress);
int readRentalFromBFile(FILE* fp, Rental* pRental, Vehicle** vehicleArr, int numVehicles);

Vehicle** readVehicleArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numVehicles);
Customer* readCustomersArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numCustomers);
Rental* readRentalArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numRentals);
int readBranchListFromBFile(FILE* fp, RentalCompany* pCompany, int* numBranches);
