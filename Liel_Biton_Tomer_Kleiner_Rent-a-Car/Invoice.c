#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Macros.h"
#include "Invoice.h"
static int invoiceSN_generator = START_SN_INVOICE;

int createInvoice(Invoice* newInvoice, float amount, int rentalSerial)
{
	ERROR_ALOC_RETURN_NULL(newInvoice)
	newInvoice->invoiceSN = invoiceSN_generator++;
	newInvoice->totalAmount = amount;
	newInvoice->rentalSN = rentalSerial;
	printf("Enter issue date: ");
	getCorrectDate(&newInvoice->issueDate);
	return 1;
}

int updateInvoiceGenerator(int num)
{
	invoiceSN_generator = num;
	return ++invoiceSN_generator;
}

int getCurrentInvoiceGenerator()
{
	return invoiceSN_generator;
}

void updateInvoice(Invoice* invoice, float updateCost)
{
	invoice->totalAmount = updateCost;
}

void printInvoice(const Invoice* invoice)
{
	printf("Invoice Number %d For Rental Number %d\nTotal Amount: %.2f\tIssue Date: ", 
		invoice->invoiceSN, invoice->rentalSN, invoice->totalAmount);
	printDate(&invoice->issueDate);
	printf("\n");
}
