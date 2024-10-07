#pragma once
#define START_SN_INSUR 0

typedef enum { eLiability, eCollision, eRoadside, ePersonal, eComprehensive, eNumTypes } eType;
static const char* types[eNumTypes] = { "Liability", "Collision Damage Waiver", "Roadside Assistance", "Personal Accident", "Comprehensive" };
static const int costs[eNumTypes] = { 12,20,18,22,30 };

typedef struct {
    unsigned int InsuranceSN; // -> From 0 - 9999 (max rentals limit) -> 14 bits
    eType type; // 5 option for type -> 3 bits
    unsigned int costPerDay; // Max cost per day is 30 -> 5 bits
    // Total 3 bytes for compressed file
}Insurance;

int createInsurance(Insurance* insurance);
eType getInsuranceType();
unsigned int updateInsuranceGenerator(unsigned int num);
unsigned int getCurrentInsuranceGenerator();
const char* GetTypeStr(int type);
void printInsurance(const Insurance* insurance);

//nothing to free