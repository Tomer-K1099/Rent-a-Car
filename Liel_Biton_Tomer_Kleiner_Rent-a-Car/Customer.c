#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "Macros.h"
#include "Customer.h"
#include "General.h"

int initCustomer(Customer* customer, const Customer* customerArr, int numCustomers)
{
    if (!getCustomerID(customer, customerArr, numCustomers)) return 0;
    if (!getCustomerFullName(customer)) return 0;
    if (!getPhoneNumber(customer)) return 0;
    customer->age = getAge();
    return 1;
}

int checkIfAllDigits(const char* Number, int len)
{
    for (int i = 0; i < len; i++) {
        if (!isdigit(Number[i]))
            return 0; // Not all characters are digits
    }
    return 1;
}

int checkUniqueID(const char* id, const Customer* customerArr, int numCustomers)
{
    for (int i = 0; i < numCustomers; i++)
    {
        if (strcmp(customerArr[i].ID, id) == 0)
            return 0;
    }
    return 1;
}

int getCustomerID(Customer* customer, const Customer* customerArr, int numCustomers)
{
    char id[MAX_STR_LEN];
    int ok = 1;
    do {
        ok = 1;
        printf("Enter ID - %d numbers\n", ID_LEN);
        myGets(id, MAX_STR_LEN, stdin);
        if (strlen(id) != ID_LEN)
        {
            printf("ID should be 9 numbers. Try again.\n");
            ok = 0;
        }
        else if (!checkIfAllDigits(id, ID_LEN))
        {
            printf("ID should contain numbers only. Try again.\n");
            ok = 0;
        }
        else if (customerArr != NULL) {                                   
            ok = checkUniqueID(id, customerArr, numCustomers);
            if (!ok)                                                       
                printf("This customer already exists.\n");
        }
    } while (!ok);
    strcpy(customer->ID, id);
    return 1;
}

int getPhoneNumber(Customer* customer)
{
    char phone[MAX_STR_LEN];
    int ok = 1;
    do {
        ok = 1;
        printf("\nEnter Phone 05******** - %d numbers\n", PHONE_LEN);
        myGets(phone, MAX_STR_LEN, stdin);
        if (strlen(phone) != PHONE_LEN) {
            printf("Phone number should be %d numbers\n", PHONE_LEN);
            ok = 0;
        }
        // Check if the phone number contains only digits
        else if (!checkIfAllDigits(phone, PHONE_LEN)) {
            printf("Phone number should contain digits only\n");
            ok = 0;
        }
        else if (strncmp(phone, "05", 2) != 0) {
                printf("Phone number should start with '05'\n");
                ok = 0;
        }
    } while (!ok);

    strcpy(customer->phone, phone);
    return 1;
}

int getCustomerFullName(Customer* customer)
{
    customer->firstName = getStrExactName("\nEnter first name");
    ERROR_ALOC_RETURN_NULL(customer->firstName)
    customer->firstName[0] = toupper(customer->firstName[0]);
    customer->lastName = getStrExactName("\nEnter last name");
    ERROR_ALOC_RETURN_NULL(customer->lastName)
    customer->lastName[0] = toupper(customer->lastName[0]);
    return 1;
}

int getAge()
{
    double age;
    do {
        printf("Enter age - minimum %d\n", MIN_AGE);
        while (!scanf("%lf", &age));

        if (age < MIN_AGE)
            printf("Not above minimum age for rental.\n");
    } while (age < MIN_AGE);
    return (int)round(age);
}

void printCustomer(const Customer* customer)
{
    printf("Customer %s %s\nID %s\tAge %d\tPhone number %s\n", 
        customer->firstName, customer->lastName, customer->ID, customer->age, customer->phone);
}

void freeCustomer(Customer* customer)
{
    free(customer->firstName);
    free(customer->lastName);
}