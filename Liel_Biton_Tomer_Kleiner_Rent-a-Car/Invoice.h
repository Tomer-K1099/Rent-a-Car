#pragma once
#define START_SN_INVOICE 0
#include "Date.h"

typedef struct {
    int invoiceSN;
    int rentalSN;
    float totalAmount;
    Date issueDate;
}Invoice;

int createInvoice(Invoice* newInvoice, float amount, int rentalSerial);
int updateInvoiceGenerator(int num);
int getCurrentInvoiceGenerator();
void printInvoice(const Invoice* invoice);
void updateInvoice(Invoice* invoice, float updateCost);

//nothing to free