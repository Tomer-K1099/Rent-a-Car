#pragma once

#define MAX_PLATE 10
#define MIN_MAN_YEAR 2018
#define MAX_MAN_YEAR 2024
#define DAY_PRICE 20
#define PREM_multiplier 3
#define STAN_multiplier 2
#define COMP_multiplier 1
#define MIN_SEATS 2
#define MAX_SEATS 10

typedef enum { ePremium, eStandard, eCompact, eNofCats} eCategory;
static char* Categories[eNofCats] = { "Premium", "Standard", "Compact" };

typedef enum { eNoFeatures, eLuxuryInterior, eEnhancedTech, eAdvancedSafety,eAll, eNofOpt } eFeatures;
static char* Features[eNofOpt] = { "No Features", "Luxury Interior", "Enhanced Technology","Advanced Safety", "All Features" };

typedef enum { eMercedesBenz, eAudi, eLexus, eNumPremiumBrands } ePremiumBrand;
static char* PremiumBrand[eNumPremiumBrands] = { "Mercedes Benz", "Audi", "Lexus"};

typedef enum { eToyota, eHonda, eFord, eNumStandardBrands } eStandardBrand;
static char* StandardBrand[eNumStandardBrands] = { "Toyota", "Honda", "Ford" };

typedef enum { eVolkswagen, eKia, eNissan, eHyundai, eNumCompactBrands } eCompactBrand;
static char* CompactBrand[eNumCompactBrands] = { "Volkswagen", "Kia", "Nissan", "Hyundai"};

typedef struct {
	int isElectric;
    eFeatures feature;
    ePremiumBrand brand;
}Premium;

typedef struct {
	double cargoCapacity;
    eStandardBrand brand;
}Standard;

typedef struct {
	double fuelEfficiency;
    eCompactBrand brand;
}Compact;

typedef struct Vehicle_ {

    void (*print)(const struct Vehicle_*); // Function pointer to print vehicle

    int vehicleSN;
    int numSeats;
    int gearBox;
    int isTaken;
    int year;
    double odometer;
    char licensePlate[MAX_PLATE + 1];
    int costPerDay;
    eCategory categoryType;

    // Union to hold different categories
    union {
        Premium  premium;
        Standard standard;
        Compact compact;
    }category;


}Vehicle;

//polymorphism functions.
void initPremium(Vehicle** vehicleArr, Vehicle* premium, int vehicleCount);
void initStandard(Vehicle** vehicleArr, Vehicle* standard, int vehicleCount);
void initCompact(Vehicle** vehicleArr, Vehicle* compact, int vehicleCount);
void initVehicle(Vehicle** vehicleArr, Vehicle* vehicle, int vehicleCount);
void printVehicle(const Vehicle* vehicle);
void printPremium(const Vehicle* premium);
void printStandard(const Vehicle* standard);
void printCompact(const Vehicle* compact);
void printVehicleV(void* vehicle);

void getLicensePlate(Vehicle* vehicle);
int getNumSeats();
int getVehicleYear();
int getVehicleSN(Vehicle** vehicleArr, int vehicleCount);
int getVehicleBrand(char** arrName, int numOfOpt);
eFeatures getPremiumFeature();

void updateOdometer(Vehicle* vehicle, int totalDays, int kmPerDay);
int rentVehicle(Vehicle* vehicle);
int checkUniqueSN(int SN, Vehicle** vehicleArr, int vehicleCount);
int checkUniquePlate(char* plate, Vehicle** vehicleArr, int vehicleCount);

int compareByOdometer(const void* v1, const void* v2);
int compareBySN(const void* v1, const void* v2);
int compareByYear(const void* v1, const void* v2);
int compareByLicensePlate(const void* v1, const void* v2);
void freeVehiclePtr(void* pVehiclePtr);

//nothing to free