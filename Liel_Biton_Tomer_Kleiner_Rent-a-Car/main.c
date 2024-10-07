#include <stdio.h>
#include <stdlib.h>

#include "RentalCompany.h"
#include "CompanyBinFiles.h"
#include "CompanyTextFiles.h"

typedef enum
{
	ePrintCompany, ePrintBranches, ePrintVehicles,
	ePrintCustomers, ePrintRentals, eAddVehicle,eAddCustomer,eAddRental,eAddBranch,eSortVehicles, 
	eFindVehicle, ePrintRentalsInBranch,eUpdateRental, eLottery,eCalculateRevenue,eNofOptionsMenu
} eMenuOptions;

const char* str[eNofOptionsMenu] = { "Print Rental Company", "Print All Branches", "Print All Vehicles",
	"Print All Customers", "Print All Rentals", "Add New Vehicle To Company","Add New Customer","Add New Rental","Add New Branch","Sort Vehicles Array",
	"Find Vehicle", "Print All Rentals In specific Branch","Update Existing Rental", "Give Discount To A Random Rental", "See Company Revenue In Specific Year"};

#define EXIT -1
int menu();
int loadDataMenu();

int main()
{
	RentalCompany company;
	int option, ok = 0;
	int stop = 0;

	do {
		stop = 1;
		option = loadDataMenu();
		switch (option) {
		case 1:
			ok = initCompanyFromTFile(&company, "rental_company.txt");
			break;
		case 2:
			ok = initCompanyFromBFile(&company, "rental_company.bin");
			break;
		case 3:
			ok = initCompany(&company);
			break;
		default:
			printf("Invalid option\n");
			stop = 0;
			break;
		}
	} while (!stop);

	if (!ok) //If failed to initialize from files 
	{
		printf("Error in initialize. Please restart manually\n");
		initCompany(&company);
	}
	do
	{
		stop = 0;
		option = menu();
		switch (option)
		{
		case ePrintCompany:
			printCompany(&company);
			break;

		case ePrintBranches:
			printAllBranches(&company);
			break;

		case ePrintVehicles:
			printAllVehicles(&company);
			break;

		case ePrintCustomers:
			printAllCustomers(&company);
			break;

		case ePrintRentals:
			printAllRentals(&company);
			break;

		case eAddVehicle:
			addVehicle(&company);
			break;

		case eAddCustomer:
			addCustomer(&company);
			break;

		case eAddRental:
			addRental(&company);
			break;

		case eAddBranch:
			addBranch(&company);
			break;

		case eSortVehicles:
			sortVehicles(&company);
			break;

		case eFindVehicle:
			findVehicle(&company);
			break;

		case ePrintRentalsInBranch:
			printRentalsByBranch(&company);
			break;

		case eUpdateRental:
			updateRental(&company);
			break;

		case eLottery:
			RentalLotteryDiscount(&company);
			break;

		case eCalculateRevenue:
			CalculateRevenueForSpecificYear(&company);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Invalid option\n");
			break;
		}
	} while (!stop);

	saveCompanyToBFile(&company, "rental_company.bin");
	saveCompanyToTFile(&company, "rental_company.txt");
	freeCompany(&company);
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptionsMenu; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int loadDataMenu()
{
	int option;
	printf("Choose option for company Initial Setup:\n");
	printf("1 - Load data from text file\n");
	printf("2 - Load data from binary file\n");
	printf("3 - Insert data manually\n");
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}