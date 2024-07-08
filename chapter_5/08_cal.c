#include <stdio.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day) {
  int i, leap;

  if (year < 1) {
    printf("Only year values less than 1 invalid.\n");
    return -1;
  }

  if (month > 12 || month < 1) {
    printf("Only month values greater than 12 or less than 1 invalid.\n");
    return -1;
  }

  if (day > 31 || day < 1) {
    printf("Only day values greater than 31 or less than 1 invalid.\n");
    return -1;
  }

  leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
  for (i = 1; i < month; i++)
    day += daytab[leap][i];

  return day;
}

/* month_day: set month, day from day of year */
int month_day(int year, int yearday, int *pmonth, int *pday) {
  int i, leap;

  if (yearday > 365 || yearday < 1) {
    printf("Only yearday values greater than 365 or less than 1 invalid.\n");
    return -1;
  }

  if (year < 1) {
    printf("Only year values less than 1 invalid.\n");
    return -1;
  }

  leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
  for (i = 1; yearday > daytab[leap][i]; i++)
    yearday -= daytab[leap][i];

  *pmonth = i;
  *pday = yearday;

  return 0;
}

/*
 * Exercise 5-8. There is no error checking in day_of_year or month_day.
 * Remedy this defect.
 */
int main() {
  int year = 2024;
  int month = 7;
  int day = 7;

  int yearday = day_of_year(year, month, day);

  if (yearday < 1)
    return -1;

  printf("Current day of the year for %d/%d/%d: %d\n", day, month, year,
         yearday);

  int pmonth = 0;
  int pday = 0;
  if (month_day(year, yearday, &pmonth, &pday) == -1)
    return -1;

  printf("Today is day %d of the year: %d/%d/%d\n", yearday, year, pmonth, pday);

  return 0;
}
