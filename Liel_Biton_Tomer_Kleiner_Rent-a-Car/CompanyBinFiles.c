#include <string.h>
#include "CompanyBinFiles.h"
#include "Macros.h"
typedef unsigned char BYTE;

int saveCompanyToBFile(const RentalCompany* pCompany, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp) return 0;

	//write company name to file
	int len = (int)strlen(pCompany->companyName) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		CLOSE_RETURN_NULL(fp)
	if (fwrite(pCompany->companyName, sizeof(char), len, fp) != len)
		CLOSE_RETURN_NULL(fp)
	//write vehicle array to file
	if (!writeVehicleArrayToBFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write customer array to file
	if (!writeCustomersArrayToBFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write rental array to file
	if (!writeRentalArrayToBFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write branch list to file
	if (!writeBranchListToBFile(fp, pCompany, pCompany->branch_list.head.next))
		CLOSE_RETURN_NULL(fp)

	fclose(fp);
	return 1;
}

int initCompanyFromBFile(RentalCompany* pCompany, const char* fileName)
{
	//open file to read
	FILE* fp = fopen(fileName, "rb");
	if (!fp) return 0;

	int	numVehicles, numCustomers, numRentals,numBranches, nameLen;
	//read name
	if (fread(&nameLen, sizeof(int), 1, fp) != 1) return 0;
	pCompany->companyName = (char*)malloc(nameLen * sizeof(char));
	if (!pCompany->companyName) return 0;
	if (fread(pCompany->companyName, sizeof(char), nameLen, fp) != nameLen)
		CLOSE_FILE_AND_FREE(pCompany->companyName, fp)
		//read vehicle array
	if ((pCompany->vehicleArr = readVehicleArrayFromBFile(fp, pCompany, &numVehicles)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numVehicles = numVehicles;
	//read customer array
	if ((pCompany->customerArr = readCustomersArrayFromBFile(fp, pCompany, &numCustomers)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numCustomers = numCustomers;
	//read rental array
	if ((pCompany->rentalArr = readRentalArrayFromBFile(fp, pCompany, &numRentals)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numRentals = numRentals;
	//read branch list
	if (!readBranchListFromBFile(fp, pCompany, &numBranches))
		CLOSE_RETURN_NULL(fp)
	pCompany->numBranches = numBranches;

	//no sorting
	pCompany->sortBy = 0;
	fclose(fp);
	return 1;
}

int writeVehicleArrayToBFile(FILE* fp, const RentalCompany* pCompany)
{
	//write vehicle number to file
	if (fwrite(&(pCompany->numVehicles), sizeof(int), 1, fp) != 1)
		return 0;
	//write vehicle array to file
	for (int i = 0; i < pCompany->numVehicles; i++)
	{
		if (!writeVehicleToBFile(fp, pCompany->vehicleArr[i]))
			return 0;
	}
	return 1;
}

int writeCustomersArrayToBFile(FILE* fp, const RentalCompany* pCompany)
{
	//write customer number to file
	if (fwrite(&(pCompany->numCustomers), sizeof(int), 1, fp) != 1)
		return 0;
	//write customer array to file
	for (int i = 0; i < pCompany->numCustomers; i++)
	{
		if (!writeCustomerToBFile(fp, &pCompany->customerArr[i]))
			return 0;
	}
	return 1;
}

int writeRentalArrayToBFile(FILE* fp, const RentalCompany* pCompany)
{
	//write rental number to file
	if (fwrite(&(pCompany->numRentals), sizeof(int), 1, fp) != 1)
		return 0;
	//write rentals array to file
	for (int i = 0; i < pCompany->numRentals; i++)
	{
		if (!writeRentalToBFile(fp, &pCompany->rentalArr[i]))
			return 0;
	}
	return 1;
}

int writeBranchListToBFile(FILE* fp, const RentalCompany* pCompany, NODE* head)
{
	NODE* current = head;
	if (fwrite(&pCompany->numBranches, sizeof(int), 1, fp) != 1)
		return 0;
	// Write each node (branch) to the file
	while (current != NULL) {
		if (!writeBranchToBFile(fp, (Branch*)current->key)) return 0;
		current = current->next;
	}
	return 1;
}

int writeVehicleToBFile(FILE* fp, Vehicle* pVehicle)
{
	// Write the non-union variables to file
	if (fwrite(pVehicle, sizeof(Vehicle) - sizeof(pVehicle->category), 1, fp) != 1)
		return 0;

	// Write the union based on category type
	switch (pVehicle->categoryType) {
	case ePremium:
		if (fwrite(&(pVehicle->category.premium), sizeof(Premium), 1, fp) != 1)
			return 0;
		break;
	case eStandard:
		if (fwrite(&(pVehicle->category.standard), sizeof(Standard), 1, fp) != 1)
			return 0;
		break;
	case eCompact:
		if (fwrite(&(pVehicle->category.compact), sizeof(Compact), 1, fp) != 1)
			return 0;
		break;
	default:
		return 0;
	}
	return 1;
}

int writeCustomerToBFile(FILE* fp, Customer* pCustomer)
{
	int lenFirst = (int)strlen(pCustomer->firstName) + 1;
	int lenLast = (int)strlen(pCustomer->lastName) + 1;

	//write first name to file
	if (fwrite(&lenFirst, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pCustomer->firstName, sizeof(char), lenFirst, fp) != lenFirst)
		return 0;
	//write last name to file
	if (fwrite(&lenLast, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pCustomer->lastName, sizeof(char), lenLast, fp) != lenLast)
		return 0;
	//write customer ID to file
	if (fwrite(pCustomer->ID, sizeof(char), ID_LEN, fp) != ID_LEN)
		return 0;
	//write customer phone to file
	if (fwrite(pCustomer->phone, sizeof(char), PHONE_LEN, fp) != PHONE_LEN)
		return 0;
	//write customer age to file
	if (fwrite(&pCustomer->age, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int writeBranchToBFile(FILE* fp, Branch* pBranch)
{
	//write branch address to file
	if (!writeAddressToBFile(fp, &pBranch->address))
		return 0;
	//write branch ID to file
	if (fwrite(&pBranch->branchID, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int writeAddressToBFile(FILE* fp, Address* pAddress)
{
	int lenCity = (int)strlen(pAddress->city) + 1;
	int lenStreet = (int)strlen(pAddress->street) + 1;

	//write city to file
	if (fwrite(&lenCity, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pAddress->city, sizeof(char), lenCity, fp) != lenCity)
		return 0;
	//write street to file
	if (fwrite(&lenStreet, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pAddress->street, sizeof(char), lenStreet, fp) != lenStreet)
		return 0;
	//write street number to file
	if (fwrite(&pAddress->number, sizeof(int), 1, fp) != 1)
		return 0;
	//write country (enum) to file
	if (fwrite(&pAddress->country, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;

}

int writeRentalToBFile(FILE* fp, Rental* pRental)
{
	//write rental serial number to file
	if (fwrite(&pRental->rentalSN, sizeof(int), 1, fp) != 1)
		return 0;
	//write serial number of the vehicle inside rental (when read, finds the right vehicle)
	if (fwrite(&pRental->vehicle->vehicleSN, sizeof(int), 1, fp) != 1)
		return 0;
	//write customer in rental
	if (!writeCustomerToBFile(fp, &pRental->customer))
		return 0;
	//write start&end dates
	if (fwrite(&pRental->startDate, sizeof(Date), 1, fp) != 1)
		return 0;
	if (fwrite(&pRental->endDate, sizeof(Date), 1, fp) != 1)
		return 0;
	//write total cost to file
	if (fwrite(&pRental->totalCost, sizeof(float), 1, fp) != 1)
		return 0;
	//write insurance of rental to file
	if (!writeCompressedInsuranceToBFile(fp, &pRental->insurance))
		return 0;
	//write rental invoice to file
	if (fwrite(&pRental->invoice, sizeof(Invoice), 1, fp) != 1)
		return 0;
	//write branch ID to file
	if (fwrite(&pRental->branchID, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int writeCompressedInsuranceToBFile(FILE* fp, Insurance* pInsurance)
{
	BYTE data[3] = { 0 };
	data[0] = pInsurance->InsuranceSN >> 8;
	data[1] = pInsurance->InsuranceSN;
	data[2] = pInsurance->type | (pInsurance->costPerDay << 3);

	if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
		return 0;
	return 1;
}

int readVehicleFromBFile(FILE* fp, Vehicle* pVehicle)
{

	// Read the non-union variables from file
	if (fread(pVehicle, sizeof(Vehicle) - sizeof(pVehicle->category), 1, fp) != 1)
		return 0;

	// Read the union based on category type
	switch (pVehicle->categoryType) {
	case ePremium:
		if (fread(&(pVehicle->category.premium), sizeof(Premium), 1, fp) != 1)
			return 0;
		pVehicle->print = printPremium;
		break;
	case eStandard:
		if (fread(&(pVehicle->category.standard), sizeof(Standard), 1, fp) != 1)
			return 0;
		pVehicle->print = printStandard;
		break;
	case eCompact:
		if (fread(&(pVehicle->category.compact), sizeof(Compact), 1, fp) != 1)
			return 0;
		pVehicle->print = printCompact;
		break;
	default:
		return 0;
	}
	return 1;
}

int readCompressedInsuranceFromBFile(FILE* fp, Insurance* pInsurance)
{
	BYTE data[3];
	if (fread(data, sizeof(char), 3, fp) != 3)
		return 0;

	pInsurance->InsuranceSN = data[1] | ((data[0] & 0x3F) << 8);
	pInsurance->type = data[2] & 7;
	pInsurance->costPerDay = data[2] >> 3;

	if (pInsurance->InsuranceSN >= getCurrentInsuranceGenerator())
		updateInsuranceGenerator(pInsurance->InsuranceSN);
	return 1;
}

int readCustomerFromBFile(FILE* fp, Customer* pCustomer)
{
	int lenFirst, lenLast;
	//read first name
	if (fread(&lenFirst, sizeof(int), 1, fp) != 1)
		return 0;
	pCustomer->firstName = (char*)malloc(lenFirst * sizeof(char));
	ERROR_ALOC_RETURN_NULL(pCustomer->firstName)

	if (fread(pCustomer->firstName, sizeof(char), lenFirst, fp) != lenFirst)
		return 0;

	//read last name
	if (fread(&lenLast, sizeof(int), 1, fp) != 1)
		return 0;
	pCustomer->lastName = (char*)malloc(lenLast * sizeof(char));
	ERROR_ALOC_RETURN_NULL(pCustomer->lastName)

	if (fread(pCustomer->lastName, sizeof(char), lenLast, fp) != lenLast)
		return 0;

	if (fread(&pCustomer->ID, sizeof(char), ID_LEN, fp) != ID_LEN)
		return 0;
	pCustomer->ID[ID_LEN] = '\0';
	if (fread(&pCustomer->phone, sizeof(char), PHONE_LEN, fp) != PHONE_LEN)
		return 0;
	pCustomer->phone[PHONE_LEN] = '\0';
	if (fread(&pCustomer->age, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int readAddressFromBFile(FILE* fp, Address* pAddress)
{
	int lenCity, lenStreet;
	//read city
	if (fread(&lenCity, sizeof(int), 1, fp) != 1)
		return 0;
	pAddress->city = (char*)malloc(lenCity * sizeof(char));
	ERROR_ALOC_RETURN_NULL(pAddress->city)

	if (fread(pAddress->city, sizeof(char), lenCity, fp) != lenCity)
		return 0;
	//read street
	if (fread(&lenStreet, sizeof(int), 1, fp) != 1)
		return 0;
	pAddress->street = (char*)malloc(lenStreet * sizeof(char));
	ERROR_ALOC_RETURN_NULL(pAddress->street)

	if (fread(pAddress->street, sizeof(char), lenStreet, fp) != lenStreet)
		return 0;

	if (fread(&pAddress->number, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&pAddress->country, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int readBranchFromBFile(FILE* fp, Branch* pBranch)
{
	//read address
	if (!readAddressFromBFile(fp, &pBranch->address))
		return 0;
	//read branch ID
	if (fread(&pBranch->branchID, sizeof(int), 1, fp) != 1)
		return 0;
	if (pBranch->branchID >= getCurrentBranchGenerator())
		updateBranchGenerator(pBranch->branchID);
	return 1;
}


int readRentalFromBFile(FILE* fp, Rental* pRental, Vehicle** vehicleArr, int numVehicles)
{
	int vehicleSN;
	Vehicle* vehicle;
	//read rental SN
	if (fread(&pRental->rentalSN, sizeof(int), 1, fp) != 1) return 0;
	if (pRental->rentalSN >= getCurrentRentalGenerator())
		updateRentalGenerator(pRental->rentalSN);
	//read vehicleSN and find the Vehicle
	if (fread(&vehicleSN, sizeof(int), 1, fp) != 1) return 0;
	vehicle = findVehicleBySN(vehicleArr, numVehicles, vehicleSN);
	if (vehicle == NULL) return 0;
	pRental->vehicle = vehicle;
	// read customer
	if (!readCustomerFromBFile(fp,&pRental->customer)) return 0;
	//read start&end dates
	if (fread(&pRental->startDate, sizeof(Date), 1, fp) != 1) return 0;
	if (fread(&pRental->endDate, sizeof(Date), 1, fp) != 1) return 0;
	//read total cost
	if (fread(&pRental->totalCost, sizeof(float), 1, fp) != 1) return 0;
	//read insurance
	if (!readCompressedInsuranceFromBFile(fp, &pRental->insurance)) return 0;
	//read invoice
	if (fread(&pRental->invoice, sizeof(Invoice), 1, fp) != 1) return 0;
	if (pRental->invoice.invoiceSN >= getCurrentInvoiceGenerator())
		updateInvoiceGenerator(pRental->invoice.invoiceSN);
	//read branch ID
	if (fread(&pRental->branchID, sizeof(int), 1, fp) != 1) return 0;

	return 1;
}


Vehicle** readVehicleArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numVehicles)
{
	if (fread(numVehicles, sizeof(int), 1, fp) != 1)
		return NULL;

	Vehicle** arr = (Vehicle**)malloc(*numVehicles * sizeof(Vehicle*));
	ERROR_ALOC_RETURN_NULL(arr);

	for (int i = 0; i < *numVehicles; i++) {
		arr[i] = (Vehicle*)malloc(sizeof(Vehicle));
		ERROR_ALOC_RETURN_NULL(arr[i]);
		if (!readVehicleFromBFile(fp, arr[i]))
		{
			freeVehicleArray(pCompany->vehicleArr, i);
			return NULL;
		}
	}
	return arr;
}

Customer* readCustomersArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numCustomers)
{
	if (fread(numCustomers, sizeof(int), 1, fp) != 1)
		return NULL;

	Customer* arr = (Customer*)malloc(*numCustomers * sizeof(Customer));
	ERROR_ALOC_RETURN_NULL(arr);

	for (int i = 0; i < *numCustomers; i++) {
		if (!readCustomerFromBFile(fp, &arr[i]))
			return NULL;
	}
	return arr;
}

Rental* readRentalArrayFromBFile(FILE* fp, RentalCompany* pCompany, int* numRentals)
{
	if (fread(numRentals, sizeof(int), 1, fp) != 1)
		return NULL;
	Rental* arr = (Rental*)malloc(*numRentals * sizeof(Rental));
	if (!arr)
		return NULL;

	for (int i = 0; i < *numRentals; i++) {
		if (!readRentalFromBFile(fp, &arr[i], pCompany->vehicleArr, pCompany->numVehicles))
			return NULL;
	}
	return arr;
}

int readBranchListFromBFile(FILE* fp, RentalCompany* pCompany, int* numBranches)
{
	if (!L_init(&pCompany->branch_list))
		return 0;
	NODE* head = &pCompany->branch_list.head;
	if (fread(numBranches, sizeof(int), 1, fp) != 1)
		return 0;
	// Read each node from the file
	for (int i = 0; i < *numBranches; i++)
	{
		Branch* pBranch = (Branch*)malloc(sizeof(Branch));
		ERROR_ALOC_RETURN_NULL(pBranch)

		// Read the branch itself
		if (!readBranchFromBFile(fp, pBranch)) {
			free(pBranch);
			return 0;
		}

		// Create a new node and add it to the list
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		ERROR_ALOC_RETURN_NULL(newNode)
		newNode->key = pBranch;
		newNode->next = NULL;
		head = L_insert(head, pBranch);

	}
	return 1;
}