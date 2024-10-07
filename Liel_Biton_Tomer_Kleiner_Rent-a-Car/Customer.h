#pragma once

#define PHONE_LEN 10
#define ID_LEN 9
#define MIN_AGE 21

typedef struct {
    char ID[ID_LEN + 1];
    char* firstName;
    char* lastName;
    char phone[PHONE_LEN + 1];
    int age;
}Customer;

int initCustomer(Customer* customer,const Customer* customerArr, int numCustomers);
int getCustomerID(Customer* customer, const Customer* customerArr, int numCustomers);
int getPhoneNumber(Customer* customer);
int getCustomerFullName(Customer* customer);
int checkUniqueID(const char* id, const Customer* customerArr, int numCustomers);
int getAge();
int checkIfAllDigits(const char* Number, int len);
void printCustomer(const Customer* customer);
void freeCustomer(Customer* customer);