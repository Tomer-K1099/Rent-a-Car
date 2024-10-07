#pragma once

#define MAX_MONTHS 12
#define MIN_MONTHS 1
#define MIN_YEAR 2024 //current year
#define MAX_DATE_LEN 10
#define DAYS_IN_YEAR 365

typedef struct {
    int day;
    int month;
    int year;
}Date;

int getCorrectDate(Date* date);
int	 checkDate(char* date, Date* pDate);
int calculateDaysOfRental(const Date start,const Date end);
void printDate(const Date* date);
int dateRangesDoNotCollide(const Date* start1, const Date* end1, const Date* start2, const Date* end2); // The function checks if the period start1-end1 collides with the period start2-end2.
int checkRentDates(const Date start, const Date end);
int getTodaysDate(Date* date);
int isDatePassedOrToday(const Date* date);

//nothing to free