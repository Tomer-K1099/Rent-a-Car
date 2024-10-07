#pragma once
#include "Customer.h"
#include "Insurance.h"
#include "Vehicle.h"
#include "Invoice.h"
#include "Branch.h"

#define START_SN_RENT 0
#define MAX_SN_RENT 9999
#define AVG_KM 100

typedef struct {
    int rentalSN;
    Vehicle* vehicle;
    Customer customer;
    Date startDate;
    Date endDate;
    float totalCost;
    Insurance insurance;
    Invoice invoice;
    int branchID;
   
}Rental;

int initRental(Rental* rental, Customer* customer, const Vehicle** vehicleArr, Rental* rentalArr, int numRentals, int numVehicles, int branchID);
float calculateTotalCost(Rental* rental);
int updateRentalGenerator(int num);
int getCurrentRentalGenerator();
int isVehicleAvailableInDates(const Rental* rentalArr, int numRentals, const Vehicle* vehicle, const Date* start, const Date* end); // Returns 1 if the vehicle that was sent is available in the dates that were sent (by customer request).
void printAvailableVehicles(const Vehicle** vehicleArr, const Rental* rentalArr, int numRentals, int numVehicles, const Rental* rental);
Vehicle* findVehicleBySN(Vehicle** vehicleArr, int numVehicles, int SN);
Vehicle* getVehicleInRental(const Vehicle** vehicleArr, Rental* rentalArr, int numRentals, int numVehicles, int rentalIndex);
int endRental(Rental* rental);
void printRental(const Rental* rental);
void freeRental(Rental* rental);
