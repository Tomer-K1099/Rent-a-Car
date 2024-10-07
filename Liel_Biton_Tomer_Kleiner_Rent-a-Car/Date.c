#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Date.h"
#include "General.h"
const int DayMonths[MAX_MONTHS] = { 31,28,31,30,31,30,31 ,31,30,31,30,31 };
#define DATE_TAV '/'

int getCorrectDate(Date* pDate) //extract Date from inputted string
{
    char date[MAX_STR_LEN];
    int ok = 1;

    do {
        printf("dd%cmm%cyyyy (current year - %d)\t", DATE_TAV, DATE_TAV, MIN_YEAR);
        myGets(date, MAX_STR_LEN, stdin);
        ok = checkDate(date, pDate);
        if (!ok)
            printf("Try enter correct date.\n");
    } while (!ok);
    return 1;
}

int	 checkDate(char* date, Date* pDate)
{
    int day, month, year;
    if (strlen(date) != MAX_DATE_LEN)
        return 0;
    if ((date[2] != DATE_TAV) || (date[5] != DATE_TAV))
        return 0;
    // read the date components from the input string
    //check date: exact size, number of integers and regural date validation.
    if (sscanf(date, "%d%*c%d%*c%d", &day, &month, &year) != 3)
        return 0;
    if (month < MIN_MONTHS || month > MAX_MONTHS || year < MIN_YEAR || day < 0 || day > DayMonths[month - 1])
        return 0;

    pDate->day = day;
    pDate->month = month;
    pDate->year = year;

    return 1;
}

int calculateDaysOfRental(const Date start, const Date end)
{
    int totalDays = 0;
    if (end.month < start.month)
    {
        for (int i = start.month - 1; i < MAX_MONTHS; i++)
            totalDays += (DayMonths[i]);
        for (int i = 0; i < end.month - 1; i++)
            totalDays += (DayMonths[i]);
        if ((end.year - start.year) > 1)
            totalDays += (DAYS_IN_YEAR * (end.year - start.year - 1));
    }
    else
    {
        for (int i = start.month - 1; i < end.month - 1; i++)
            totalDays += (DayMonths[i]);
        totalDays += (DAYS_IN_YEAR * (end.year - start.year));
    }
    totalDays = totalDays - start.day + end.day;
    if (totalDays == 0) //if its the same day
        return 1;
    else
        return totalDays;
}

int checkRentDates(const Date start, const Date end)
{
    if (end.year < start.year)
        return 0;
    if (end.year == start.year && end.month < start.month)
        return 0;
    if (end.year == start.year && end.month == start.month && end.day < start.day)
        return 0;
    return 1;
}

int dateRangesDoNotCollide(const Date* start1, const Date* end1, const Date* start2, const Date* end2) // Return 0 if there is a collision.
{
    if (end1->year < start2->year ||
        (end1->year == start2->year && end1->month < start2->month) ||
        (end1->year == start2->year && end1->month == start2->month && end1->day < start2->day) ||
        start1->year > end2->year ||
        (start1->year == end2->year && start1->month > end2->month) ||
        (start1->year == end2->year && start1->month == end2->month && start1->day > end2->day)) {
        return 1; // No collision
    }
    else
        return 0; // Collision
}

int getTodaysDate(Date* date) 
{
    
    // Get current time
    time_t currentTime;
    struct tm* localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);

    // Extract day, month, and year from current time
    date->day = localTime->tm_mday;
    date->month = localTime->tm_mon + 1;
    date->year = localTime->tm_year + 1900;
    return 1;
}

int isDatePassedOrToday(const Date* date) 
{
    // Get current time
    time_t currentTime;
    struct tm* localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    time_t givenTime, currentTimeInSec;
    struct tm givenDate = { 0 };

    givenDate.tm_year = date->year - 1900;
    givenDate.tm_mon = date->month - 1;
    givenDate.tm_mday = date->day;
    givenTime = mktime(&givenDate);
    currentTimeInSec = mktime(localTime);

    // Compare given date with current date - returns 1 if the given date is the same as today or passed.
    return givenTime <= currentTimeInSec;
}

void printDate(const Date* date) {
    printf(" %d/%d/%d ", date->day, date->month, date->year);
}