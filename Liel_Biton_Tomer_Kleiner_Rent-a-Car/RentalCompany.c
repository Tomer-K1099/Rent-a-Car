#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "Macros.h"
#include "RentalCompany.h"

int initCompany(RentalCompany* company) {
	company->companyName = getStrExactName("Enter company name\n");
	ERROR_ALOC_RETURN_NULL(company->companyName)
	if (!L_init(&company->branch_list))
		return 0;
	company->customerArr = NULL;
	company->vehicleArr = NULL;
	company->rentalArr = NULL;
	company->numRentals = 0;
	company->numCustomers = 0;
	company->numVehicles = 0;
	company->numBranches = 0;
	company->sortBy = 0;
	return 1;
}
int addBranch(RentalCompany* company) {

	ERROR_ALOC_RETURN_NULL(company)
	Branch* pBranch = (Branch*)calloc(1, sizeof(Branch));
	ERROR_ALOC_RETURN_NULL(pBranch)
	if (!initBranch(pBranch)) {
		freeBranch(pBranch);
		free(pBranch);
		return 0;
	}
	NODE* lastNode = &company->branch_list.head;
	while (lastNode->next != NULL)
	{
		lastNode = lastNode->next;
	}

	L_insert(lastNode, pBranch);
	company->numBranches++;
	return 1;
}

int chooseBranch(const RentalCompany* company)
{
	int choice;
	NODE* node = company->branch_list.head.next;
	Branch* temp;
	if (!node) {
		printf("\nThere are no Branches to your company. Try adding a branch first");
		return -1;
	}
	printAllBranches(company);
	do {
		printf("Choose a branch from list, type its ID\n");
		while (!scanf("%d", &choice));
		temp = findBranchByID(company, choice);
		if (!temp)
			printf("No branch with this ID. Try again!\n");

	} while (temp == NULL);

	return temp->branchID;
}

Branch* findBranchByID(const RentalCompany* company, int branchID)  //returns NULL both on empty list and if branchID not found in list.
{
	NODE* node = company->branch_list.head.next;
	Branch* temp;
	if (!node) {
		printf("\nThere are no Branches to your company. Try adding a branch first");
		return NULL;
	}

	while (node != NULL)
	{
		temp = (Branch*)node->key;
		if (temp->branchID == branchID)
		{
			return (Branch*)node->key;
		}
		node = node->next;
	}
	return NULL;
}

Customer* chooseCustomerByID(const RentalCompany* company)
{
	Customer* temp;
	Customer toSearch;
	if (!company->numCustomers)
	{
		printf("No customers to choose from. try adding customers first.");
		return NULL;
	}
	printAllCustomers(company);
	do {
		printf("\nChoose a customer from list by typing its ID\n");
		getCustomerID(&toSearch, NULL, 0);
		temp = findCustomerByID(company->customerArr, company->numCustomers, toSearch.ID);
		if (!temp)
			printf("No customer with this ID. Try again!\n");
	} while (temp == NULL);
	return temp;
}

Customer* SelectNewOrExistingCustomer(RentalCompany* company)
{
	int option;
	Customer* customer = NULL;
	do {
		printf("\nPlease choose one of the following options\n");
		printf("1 - Create a new rental for an existing customer\n");
		printf("2 - Create a new rental for a new customer\n");
		printf("3 - Exit\n");
		while (!scanf("%d", &option));

		switch (option) {
		case 1:
			customer = chooseCustomerByID(company);
			break;
		case 2:
			addCustomer(company);
			customer = &company->customerArr[company->numCustomers - 1];
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}
	} while (option < 1 || option > 2);
	return customer;
}

int addRental(RentalCompany* company) 
{
	Customer* customer;
	customer = SelectNewOrExistingCustomer(company); //choose if new or existing customer in new rental 
	ERROR_ALOC_RETURN_NULL(customer)

	//choose branch to pick up the vehicle
	printf("\nSelect Pick-up location\n");
	int branchID = chooseBranch(company);
	if (branchID == -1) return 0; //no branches in comapny.

	company->rentalArr = (Rental*)realloc(company->rentalArr, (company->numRentals + 1) * sizeof(Rental));
	ERROR_ALOC_RETURN_NULL(company->rentalArr)
	if (!initRental(&company->rentalArr[company->numRentals], customer, company->vehicleArr, company->rentalArr, company->numRentals, company->numVehicles, branchID))
		return 0;
	rentVehicle(company->rentalArr[company->numRentals].vehicle);
	company->rentalArr[company->numRentals].totalCost = calculateTotalCost(&company->rentalArr[company->numRentals]);
	updateInvoice(&company->rentalArr[company->numRentals].invoice, company->rentalArr[company->numRentals].totalCost);
	company->numRentals++;
	return 1;
}

int addVehicle(RentalCompany* company) {
	int choice = 0;

	Vehicle* pVehicle = (Vehicle*)malloc(sizeof(Vehicle));
	ERROR_ALOC_RETURN_NULL(pVehicle)

	do {
		printf("Please choose vehicle category to add\n");
		for (int i = 0; i < eNofCats; i++)
			printf("[%d] for %s\n", i + 1, Categories[i]);
	} while (!scanf("%d", &choice) || choice<=0 || choice>eNofCats);
	choice--;
	switch (choice) {
	case ePremium:
		initPremium(company->vehicleArr, pVehicle, company->numVehicles);
		break;
	case eStandard:
		initStandard(company->vehicleArr, pVehicle, company->numVehicles);
		break;
	case eCompact:
		initCompact(company->vehicleArr, pVehicle, company->numVehicles);
		break;
	default:
		break;
	}

	company->vehicleArr = (Vehicle**)realloc(company->vehicleArr, ((company->numVehicles + 1) * sizeof(Vehicle*)));
	if (!company->vehicleArr)
	{
		free(pVehicle);
		return 0;
	}
	company->vehicleArr[company->numVehicles] = pVehicle;
	company->numVehicles++;
	return 1;
}

int addCustomer(RentalCompany* company)
{
	company->customerArr = (Customer*)realloc(company->customerArr, (company->numCustomers + 1) * sizeof(Customer));
	ERROR_ALOC_RETURN_NULL(company->customerArr)
	initCustomer(&company->customerArr[company->numCustomers],company->customerArr,company->numCustomers);
	company->numCustomers++;
	return 1;
}

Customer* findCustomerByID(Customer* customerArr, int numCustomers, char* ID)
{
	for (int i = 0; i < numCustomers; i++)
	{
		if (strcmp(customerArr[i].ID, ID) == 0)
			return &customerArr[i];
	}
	return NULL;
}

void RentalLotteryDiscount(RentalCompany* company)
{
	int randomIndex;
	srand((int)time(NULL));
	if (!company->numRentals)
	{
		printf("There are no rentals in the company.");
		return;
	}
	randomIndex = rand() % (company->numRentals);
	printf("The rental below got a %d%% discount!\n", DISCOUNT);
	printf("Total amount before discount: %.2f\n", company->rentalArr[randomIndex].totalCost);
	company->rentalArr[randomIndex].totalCost *= (float)0.8;
	updateInvoice(&company->rentalArr[randomIndex].invoice, company->rentalArr[randomIndex].totalCost); // Update rental invoice.
	printRental(&company->rentalArr[randomIndex]);
	printf("Sending message to customer ...");
	fflush(stdout); //Flush output buffer

	//Print dots with a delay to "simulate" loading
	for (int i = 0; i < 10; i++) {
		printf(".");
		fflush(stdout); 
		Sleep(500);      // Sleep for 0.5 second
	}
	printf(" Message sent!\n"); // Print completion message
}

int chooseIndexFromRentalArray(RentalCompany* company)
{
	int index, ok = 1;
	do {
		ok = 1;
		index = getIntegerNum("\nChoose the line number of the rental you want to update: "); // The index in rental array that user want to update.
		if (index < 0 || index >= company->numRentals)
		{
			printf("No such line. Try again\n");
			ok = 0;
		}
	} while (!ok);
	return index;
}

updateType getUpdateType()
{
	int option;
	printf("\n");
	do {
		printf("Please choose the option to update\n");
		for (int i = 0; i < eNum; i++)
			printf("\n[%d] for %s", i + 1, updateName[i]);
		printf("\n");
		scanf("%d", &option);
	} while (option <= 0 || option > eNum);
	option--;
	return (updateType)option;
}

int updateRentalHelper(RentalCompany* company)
{
	if (!company->numRentals) {
		printf("There are no rentals in the company.");
		return 0;
	}
	for (int i = 0; i < company->numRentals; i++)
	{
		printf("\n- - - - To Choose This Rental Enter %d - - - -\n", i);
		printRental(&company->rentalArr[i]);
	}
	return 1;
}

void updateRental(RentalCompany* company)
{
	int index, ok;
	if (!updateRentalHelper(company)) return;
	index = chooseIndexFromRentalArray(company);
	updateType type = getUpdateType();
	switch (type)
	{
	case eStartDate:
		do {
			getCorrectDate(&company->rentalArr[index].startDate);
			ok = checkRentDates(company->rentalArr[index].startDate, company->rentalArr[index].endDate);
		} while (!ok);
		break;
	case eEndDate:
		do {
			getCorrectDate(&company->rentalArr[index].endDate);
			ok = checkRentDates(company->rentalArr[index].startDate, company->rentalArr[index].endDate);
			if (!ok)
				printf("End date is earlier than start date. Try again.\n");
		} while (!ok);
		break;
	case eVehicle:
		company->rentalArr[index].vehicle = getVehicleInRental(company->vehicleArr, company->rentalArr, company->numRentals, company->numVehicles, index);
		break;
	case eEnd:
		endRental(&company->rentalArr[index]);
		return;
	}
	company->rentalArr[index].totalCost = calculateTotalCost(&company->rentalArr[index]); // Calculate the total cost in case of changes.
	updateInvoice(&company->rentalArr[index].invoice, company->rentalArr[index].totalCost); // Update the invoice as well.
}

void CalculateRevenueForSpecificYear(const RentalCompany* company)
{
	float totalRevenue = 0;
	int year = getIntegerNum("Enter the year for which you would like to check revenue");
	if (year > MAX_MAN_YEAR) {
		printf("A revenue for a future year is not available.\n");
		return;
	}
	for (int i = 0; i < company->numRentals; i++)
	{
		if (company->rentalArr[i].invoice.issueDate.year == year)
			totalRevenue += (company->rentalArr[i].invoice.totalAmount);
	}
	printf("The revenue in %d is %.2f\n", year, totalRevenue);
}

sortType getSortType()
{
	int option;
	printf("Based on what criteria do you want to sort?\n");
	do {
		for (int i = 1; i < eOpt; i++)
			printf("Enter %d for %s\n", i, sortName[i]);
		while(!scanf("%d", &option));
		printf("\n");
	} while (option <= 0 || option >= eOpt);
	getchar();
	return (sortType)option;
}

void sortVehicles(RentalCompany* company)
{
	if (!company) return;
	if (!company->numVehicles)
	{
		printf("No Vehicles in company. Cannot proceed with sorting.\n");
		return;
	}
	company->sortBy = getSortType();
	qsort(company->vehicleArr, company->numVehicles, sizeof(Vehicle*), compareFunctions[company->sortBy]);
	printf("The Vehicles Array has been sorted successfully.\n");
}

void askUserSearchParameter(Vehicle* toSearch, const RentalCompany* company)
{
	printf("The Vehicles Array is sorted by %s parameter. ", sortName[company->sortBy]);
	switch (company->sortBy)
	{
	case eOdometer:
		toSearch->odometer = getDoubleNum("Enter odometer: ");
		break;
	case eSerialNumber:
		toSearch->vehicleSN = getIntegerNum("Enter serial number: ");
		break;
	case eYear:
		toSearch->year = getVehicleYear();
		break;
	case eLicensePlate:
		getLicensePlate(toSearch);
		break;
	default:
		break;
	}
}

void findVehicle(const RentalCompany* company)
{
	if (company->sortBy == 0)
	{
		printf("\nThe search cannot be performed, array not sorted\n");
		return;
	}
	Vehicle* toSearch = (Vehicle*)malloc(sizeof(Vehicle));
	if (!toSearch) return;
	askUserSearchParameter(toSearch, company);
	Vehicle** pRes = (Vehicle**)bsearch(&toSearch, company->vehicleArr, company->numVehicles, sizeof(Vehicle*), compareFunctions[company->sortBy]);
	if (!pRes) printf("Vehicle was not found\n");
	else {
		printf("Vehicle found!\n");
		printVehicleV(pRes);
	}
	free(toSearch);
}

void printCompany(const RentalCompany* company)
{
	printf("\n\033[0;33m----- %s -----\033[0m\n", company->companyName);
	printf("\n\033[1;34mCompany Branches\033[0m\n");
	printAllBranches(company);
	printf("\n\033[1;34mCompany Vehicles\033[0m\n");
	printAllVehicles(company);
	printf("\n\033[1;34mCompany Customers\033[0m\n");
	printAllCustomers(company);
	printf("\n\033[1;34mCompany Rentals\033[0m\n");
	printAllRentals(company);
}

void printAllVehicles(const RentalCompany* company)
{
	generalArrayFunction(company->vehicleArr, company->numVehicles, sizeof(Vehicle*), printVehicleV);
	printf("\n");
}

void printAllCustomers(const RentalCompany* company)
{
	generalArrayFunction(company->customerArr, company->numCustomers, sizeof(Customer), printCustomer);
	printf("\n");
}

void printRentalsByBranch(const RentalCompany* company)
{
	if (!company->numRentals)
	{
		printf("There are no rentals in the company.");
		return;
	}
	int chosenBranchID = chooseBranch(company);
	for (int i = 0; i < company->numRentals; i++)
	{
		if (company->rentalArr[i].branchID == chosenBranchID)
			printRental(&company->rentalArr[i]);
	}
}

void printAllRentals(const RentalCompany* company)
{
	generalArrayFunction(company->rentalArr, company->numRentals, sizeof(Rental), printRental);
}

void printAllBranches(const RentalCompany* company)
{
	L_print(&company->branch_list, printBranch);
}

void freeVehicleArray(Vehicle** arr, int numVehicles)
{
	generalArrayFunction(arr, numVehicles, sizeof(Vehicle*), freeVehiclePtr);
}

void freeCustomerArray(Customer* arr, int numCustomers)
{
	generalArrayFunction(arr, numCustomers, sizeof(Customer), freeCustomer);
}

void freeCompany(RentalCompany* company)
{
	free(company->companyName);
	L_free(&company->branch_list, freeBranch);
	freeVehicleArray(company->vehicleArr, company->numVehicles);
	free(company->vehicleArr);
	freeCustomerArray(company->customerArr, company->numCustomers);
	free(company->customerArr);
	free(company->rentalArr);
}