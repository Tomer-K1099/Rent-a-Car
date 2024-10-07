#include <string.h>
#include "CompanyTextFiles.h"
#include "Macros.h"

int saveCompanyToTFile(const RentalCompany* pCompany, const char* fileName)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp) return 0;

	//write company name to file
	int len = (int)strlen(pCompany->companyName) + 1;
	if (fprintf(fp, "%s\n", pCompany->companyName) != len)
		CLOSE_RETURN_NULL(fp)
	//write vehicle array to file
	if (!writeVehicleArrayToTFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write customer array to file
	if (!writeCustomersArrayToTFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write rental array to file
	if (!writeRentalArrayToTFile(fp, pCompany))
		CLOSE_RETURN_NULL(fp)
	//write branch list to file
	if (!writeBranchListToTFile(fp, pCompany, pCompany->branch_list.head.next))
		CLOSE_RETURN_NULL(fp)

	fclose(fp);
	return 1;
}

int initCompanyFromTFile(RentalCompany* pCompany, const char* fileName)
{
	//open file to read
	FILE* fp = fopen(fileName, "r");
	if (!fp) return 0;
	char name[MAX_STR_LEN];
	int	numVehicles, numCustomers, numRentals, numBranches;
	//read name
	if(!myGets(name, MAX_STR_LEN, fp))
		CLOSE_RETURN_NULL(fp)
	pCompany->companyName = getDynStr(name);
	//read vehicle array
	if ((pCompany->vehicleArr = readVehicleArrayFromTFile(fp, pCompany, &numVehicles)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numVehicles = numVehicles;
	//read customer array
	if ((pCompany->customerArr = readCustomersArrayFromTFile(fp, pCompany, &numCustomers)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numCustomers = numCustomers;
	//read rental array
	if ((pCompany->rentalArr = readRentalArrayFromTFile(fp, pCompany, &numRentals)) == NULL)
		CLOSE_RETURN_NULL(fp)
		pCompany->numRentals = numRentals;
	//read branch list
	if (!readBranchListFromTFile(fp, pCompany, &numBranches))
		CLOSE_RETURN_NULL(fp)
		pCompany->numBranches = numBranches;

	//no sorting
	pCompany->sortBy = 0;
	fclose(fp);
	return 1;
}

int writeVehicleArrayToTFile(FILE* fp, const RentalCompany* pCompany)
{
	//write vehicle number to file
	if (fprintf(fp, "%d\n", pCompany->numVehicles) < 1)
		return 0;
	//write vehicle array to file
	for (int i = 0; i < pCompany->numVehicles; i++)
	{
		if (!writeVehicleToTFile(fp, pCompany->vehicleArr[i]))
			return 0;
	}
	return 1;
}

int writeCustomersArrayToTFile(FILE* fp, const RentalCompany* pCompany)
{
	//write customer number to file
	if (fprintf(fp, "%d\n", pCompany->numCustomers) < 1)
		return 0;
	//write customer array to file
	for (int i = 0; i < pCompany->numCustomers; i++)
	{
		if (!writeCustomerToTFile(fp, &pCompany->customerArr[i]))
			return 0;
	}
	return 1;
}

int writeRentalArrayToTFile(FILE* fp, const RentalCompany* pCompany)
{
	//write rental number to file
	if (fprintf(fp, "%d\n", pCompany->numRentals) < 1)
		return 0;
	//write rentals array to file
	for (int i = 0; i < pCompany->numRentals; i++)
	{
		if (!writeRentalToTFile(fp, &pCompany->rentalArr[i]))
			return 0;
	}
	return 1;
}

int writeBranchListToTFile(FILE* fp, const RentalCompany* pCompany, NODE* head)
{
	// Write number of nodes & each node (branch) to the file
	NODE* current = head;
	if (fprintf(fp, "%d\n", pCompany->numBranches) < 1)
		return 0;
	while (current != NULL) {
		if (!writeBranchToTFile(fp, (Branch*)current->key)) return 0;
		current = current->next;
	}
	return 1;
}

int writeVehicleToTFile(FILE* fp, Vehicle* pVehicle)
{
	// Write the non-union variables to file
	if (fprintf(fp, "%d\t", pVehicle->vehicleSN) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->numSeats) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->gearBox) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->isTaken) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->year) < 1)
		return 0;
	if (fprintf(fp, "%f\t", pVehicle->odometer) < 1)
		return 0;
	if (fprintf(fp, "%s\t", pVehicle->licensePlate) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->costPerDay) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pVehicle->categoryType) < 1)
		return 0;
	if (!writeVehicleUnionToTFile(fp, pVehicle)) return 0;
	return 1;
}

int writeVehicleUnionToTFile(FILE* fp, Vehicle* pVehicle)
{
	// Write the union based on category type
	switch (pVehicle->categoryType) {
	case ePremium:
		if (fprintf(fp, "%d\t%d\t%d\n", pVehicle->category.premium.isElectric, pVehicle->category.premium.feature, pVehicle->category.premium.brand) < 1)
			return 0;
		break;
	case eStandard:
		if (fprintf(fp, "%f\t%d\n", pVehicle->category.standard.cargoCapacity, pVehicle->category.standard.brand) < 1)
			return 0;
		break;
	case eCompact:
		if (fprintf(fp, "%f\t%d\n", pVehicle->category.compact.fuelEfficiency, pVehicle->category.compact.brand) < 1)
			return 0;
		break;
	default:
		return 0;
	}
	return 1;
}

int writeCustomerToTFile(FILE* fp, Customer* pCustomer)
{
	int lenFirst = (int)strlen(pCustomer->firstName) + 1;
	int lenLast = (int)strlen(pCustomer->lastName) + 1;

	//write first name to file
	if (fprintf(fp, "%s\n", pCustomer->firstName) != lenFirst)
		return 0;
	//write last name to file
	if (fprintf(fp, "%s\n", pCustomer->lastName) != lenLast)
		return 0;
	//write customer ID to file
	if (fprintf(fp, "%s\n", pCustomer->ID) < 1)
		return 0;
	//write customer phone to file
	if (fprintf(fp, "%s\n", pCustomer->phone) < 1)
		return 0;
	//write customer age to file
	if (fprintf(fp, "%d\n", pCustomer->age) < 1)
		return 0;
	return 1;
}

int writeBranchToTFile(FILE* fp, Branch* pBranch)
{
	//write branch address to file
	if (!writeAddressToTFile(fp, &pBranch->address))
		return 0;
	//write branch ID to file
	if (fprintf(fp, "%d\n", pBranch->branchID) < 1)
		return 0;
	return 1;
}

int writeAddressToTFile(FILE* fp, Address* pAddress)
{
	int lenCity = (int)strlen(pAddress->city) + 1;
	int lenStreet = (int)strlen(pAddress->street) + 1;

	//write city to file
	if (fprintf(fp, "%s\n", pAddress->city) != lenCity)
		return 0;
	//write street to file
	if (fprintf(fp, "%s\n", pAddress->street) != lenStreet)
		return 0;
	//write street number to file
	if (fprintf(fp, "%d\t", pAddress->number) < 1)
		return 0;
	//write country (enum) to file
	if (fprintf(fp, "%d\t", pAddress->country) < 1)
		return 0;
	return 1;
}

int writeRentalToTFile(FILE* fp, Rental* pRental)
{
	//write rental serial number to file
	if (fprintf(fp, "%d\t", pRental->rentalSN) < 1)
		return 0;
	//write serial number of the vehicle inside rental (when read, finds the right vehicle)
	if (fprintf(fp, "%d\t", pRental->vehicle->vehicleSN) < 1)
		return 0;
	//write customer ID  in rental 
	if (fprintf(fp, "%s\t", pRental->customer.ID) < 1)
		return 0;
	//write start&end dates
	if (!writeDateToTFile(fp, &pRental->startDate))
		return 0;
	if (!writeDateToTFile(fp, &pRental->endDate))
		return 0;
	//write total cost to file
	if (fprintf(fp, "%f\t", pRental->totalCost) < 1)
		return 0;
	//write insurance of rental to file
	if (!writeInsuranceToTFile(fp, &pRental->insurance))
		return 0;
	//write rental invoice to file
	if (!writeInvoiceToTFile(fp,&pRental->invoice))
		return 0;
	//write branch ID to file
	if (fprintf(fp, "%d\n", pRental->branchID) < 1)
		return 0;
	return 1;
}

int writeInsuranceToTFile(FILE* fp, Insurance* pInsurance)
{
	if (fprintf(fp, "%d\t", pInsurance->InsuranceSN) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pInsurance->type) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pInsurance->costPerDay) < 1)
		return 0;
	return 1;
}

int writeInvoiceToTFile(FILE* fp, Invoice* pInvoice) {

	if (fprintf(fp, "%d\t", pInvoice->invoiceSN) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pInvoice->rentalSN) < 1)
		return 0;
	if (fprintf(fp, "%f\t", pInvoice->totalAmount) < 1)
		return 0;
	if (!writeDateToTFile(fp, &pInvoice->issueDate))
		return 0;
	return 1;
}

int writeDateToTFile(FILE* fp, Date* pDate)
{
	if (fprintf(fp, "%d\t", pDate->day) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pDate->month) < 1)
		return 0;
	if (fprintf(fp, "%d\t", pDate->year) < 1)
		return 0;
	return 1;
}

int readVehicleFromTFile(FILE* fp, Vehicle* pVehicle)
{

	// Read the non-union variables from file
	if (fscanf(fp, "%d", &pVehicle->vehicleSN) != 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->numSeats) != 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->gearBox) != 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->isTaken) != 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->year) != 1)
		return 0;
	if (fscanf(fp, "%lf", &pVehicle->odometer) != 1)
		return 0;
	if (fscanf(fp, "%s", pVehicle->licensePlate) < 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->costPerDay) != 1)
		return 0;
	if (fscanf(fp, "%d", &pVehicle->categoryType) != 1)
		return 0;
	if (!readVehicleUnionFromTFile(fp, pVehicle)) return 0;
	return 1;
}

int readVehicleUnionFromTFile(FILE* fp, Vehicle* pVehicle)
{
	// Read the union based on category type
	switch (pVehicle->categoryType) {
	case ePremium:
		if (fscanf(fp, "%d", &pVehicle->category.premium.isElectric) != 1)
			return 0;
		if (fscanf(fp, "%d", &pVehicle->category.premium.feature) != 1)
			return 0;
		if (fscanf(fp, "%d", &pVehicle->category.premium.brand) != 1)
			return 0;
		pVehicle->print = printPremium;
		break;
	case eStandard:
		if (fscanf(fp, "%lf", &pVehicle->category.standard.cargoCapacity) != 1)
			return 0;
		if (fscanf(fp, "%d", &pVehicle->category.standard.brand) != 1)
			return 0;
		pVehicle->print = printStandard;
		break;

	case eCompact:
		if (fscanf(fp, "%lf", &pVehicle->category.compact.fuelEfficiency) != 1)
			return 0;
		if (fscanf(fp, "%d", &pVehicle->category.compact.brand) != 1)
			return 0;
		pVehicle->print = printCompact;
		break;
	default:
		return 0;
	}
	return 1;
}


int readCustomerFromTFile(FILE* fp, Customer* pCustomer)
{
	char name[MAX_STR_LEN];
	//read first name
	if (!myGets(name, MAX_STR_LEN, fp))
		return 0;
	pCustomer->firstName = getDynStr(name);
	//read last name
	if (!myGets(name, MAX_STR_LEN, fp))
		return 0;
	pCustomer->lastName = getDynStr(name);
	
	if (!myGets(pCustomer->ID, MAX_STR_LEN, fp))
		return 0;
	if (!myGets(pCustomer->phone, MAX_STR_LEN, fp))
		return 0;
	if (fscanf(fp, "%d", &pCustomer->age) != 1)
		return 0;
	return 1;
}

int readAddressFromTFile(FILE* fp, Address* pAddress)
{
	char temp[MAX_STR_LEN];
	//read city
	if (!myGets(temp, MAX_STR_LEN, fp))
		return 0;
	pAddress->city = getDynStr(temp);

	//read street
	if (!myGets(temp, MAX_STR_LEN, fp))
		return 0;
	pAddress->street = getDynStr(temp);

	if (fscanf(fp, "%d", &pAddress->number) != 1)
		return 0;
	if (fscanf(fp, "%d", &pAddress->country) != 1)
		return 0;
	return 1;
}

int readBranchFromTFile(FILE* fp, Branch* pBranch)
{
	//read address
	if (!readAddressFromTFile(fp, &pBranch->address))
		return 0;
	//read branch ID
	if (fscanf(fp, "%d", &pBranch->branchID) != 1)
		return 0;
	if (pBranch->branchID >= getCurrentBranchGenerator())
		updateBranchGenerator(pBranch->branchID);
	return 1;
}


int readRentalFromTFile(FILE* fp, Rental* pRental, Vehicle** vehicleArr, Customer* customerArr, int numCustomers, int numVehicles)
{
	int vehicleSN;
	char customerID[MAX_STR_LEN];
	Vehicle* vehicle;
	Customer* cust;

	//read rental SN
	if (fscanf(fp, "%d", &pRental->rentalSN) != 1)
		return 0;
	if (pRental->rentalSN >= getCurrentRentalGenerator())
		updateRentalGenerator(pRental->rentalSN);
	//read vehicleSN and find the Vehicle
	if (fscanf(fp, "%d", &vehicleSN) != 1)
		return 0;
	vehicle = findVehicleBySN(vehicleArr, numVehicles, vehicleSN);
	if (vehicle == NULL) return 0;
	pRental->vehicle = vehicle;
	// read customerID and find Customer
	if (fscanf(fp, "%s", &customerID) < 1)
		return 0;
	cust = findCustomerByID(customerArr, numCustomers, customerID);
	if (cust == NULL) return 0;
	pRental->customer = *cust;
	//read start&end dates
	if (!readDateFromTFile(fp, &pRental->startDate)) return 0;
	if (!readDateFromTFile(fp, &pRental->endDate)) return 0;
	//read total cost
	if (fscanf(fp, "%f", &pRental->totalCost) != 1)
		return 0;
	//read insurance
	if (!readInsuranceFromTFile(fp, &pRental->insurance)) return 0;
	//read invoice
	if (!readInvoiceFromTFile(fp, &pRental->invoice)) return 0;
	//read branch ID
	if (fscanf(fp, "%d", &pRental->branchID) != 1) return 0;
	return 1;
}


Vehicle** readVehicleArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numVehicles)
{
	if (fscanf(fp, "%d", numVehicles) != 1)
		return NULL;

	Vehicle** arr = (Vehicle**)malloc(*numVehicles * sizeof(Vehicle*));
	ERROR_ALOC_RETURN_NULL(arr);

	for (int i = 0; i < *numVehicles; i++) {
		arr[i] = (Vehicle*)malloc(sizeof(Vehicle));
		ERROR_ALOC_RETURN_NULL(arr[i]);
		if (!readVehicleFromTFile(fp, arr[i]))
		{
			freeVehicleArray(pCompany->vehicleArr, i);
			return NULL;
		}
	}
	return arr;
}

Customer* readCustomersArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numCustomers)
{
	if (fscanf(fp, "%d", numCustomers) != 1)
		return 0;

	Customer* arr = (Customer*)malloc(*numCustomers * sizeof(Customer));
	ERROR_ALOC_RETURN_NULL(arr);

	for (int i = 0; i < *numCustomers; i++) {
		if (!readCustomerFromTFile(fp, &arr[i]))
			return NULL;
	}
	return arr;
}

Rental* readRentalArrayFromTFile(FILE* fp, RentalCompany* pCompany, int* numRentals)
{
	if (fscanf(fp, "%d", numRentals) != 1)
		return NULL;

	Rental* arr = (Rental*)malloc(*numRentals * sizeof(Rental));
	ERROR_ALOC_RETURN_NULL(arr);

	for (int i = 0; i < *numRentals; i++) {
		if (!readRentalFromTFile(fp, &arr[i], pCompany->vehicleArr, pCompany->customerArr, pCompany->numCustomers, pCompany->numVehicles))
			return NULL;
	}
	return arr;
}

int readBranchListFromTFile(FILE* fp, RentalCompany* pCompany, int* numBranches)
{
	if (!L_init(&pCompany->branch_list))
		return 0;
	NODE* head = &pCompany->branch_list.head;
	if (fscanf(fp, "%d", numBranches) != 1)
		return 0;
	for (int i = 0; i < *numBranches; i++)
	{
		Branch* pBranch = (Branch*)malloc(sizeof(Branch));
		ERROR_ALOC_RETURN_NULL(pBranch)

		// Read the branch itself
		if (!readBranchFromTFile(fp, pBranch)) {
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
int readInsuranceFromTFile(FILE* fp, Insurance* pInsurance)
{
	if (fscanf(fp, "%d", &pInsurance->InsuranceSN) != 1)
		return 0;
	if (fscanf(fp, "%d", &pInsurance->type) != 1)
		return 0;
	if (fscanf(fp, "%d", &pInsurance->costPerDay) != 1)
		return 0;
	if (pInsurance->InsuranceSN >= getCurrentInsuranceGenerator())
		updateInsuranceGenerator(pInsurance->InsuranceSN);
	return 1;
}

int readInvoiceFromTFile(FILE* fp, Invoice* pInvoice) 
{

	if (fscanf(fp, "%d", &pInvoice->invoiceSN) != 1)
		return 0;
	if (fscanf(fp, "%d", &pInvoice->rentalSN) != 1)
		return 0;
	if (fscanf(fp, "%f", &pInvoice->totalAmount) != 1)
		return 0;
	if (!readDateFromTFile(fp, &pInvoice->issueDate))
		return 0;
	if (pInvoice->invoiceSN >= getCurrentInvoiceGenerator())
		updateInvoiceGenerator(pInvoice->invoiceSN);
	return 1;
}

int readDateFromTFile(FILE* fp, Date* pDate)
{
	if (fscanf(fp, "%d", &pDate->day) != 1)
		return 0;
	if (fscanf(fp, "%d", &pDate->month) != 1)
		return 0;
	if (fscanf(fp, "%d", &pDate->year) != 1)
		return 0;
	return 1;
}

