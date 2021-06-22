#include <time.h>
#include <sys/time.h>
#include <esp_system.h>
#include <string>

#ifndef DATETIME_H
#define DATETIME_H

struct tm dateTime;

void configureDateTime() {
  timeval tv;
  tv.tv_sec = 1624327777;
  // settimeofday(&tv, NULL);
}

std::string getFormattedDateTime() {
  time_t tt = time(NULL);
  dateTime = *gmtime(&tt);
  
  char formattedDate[64];
  strftime(formattedDate, 64, "%d/%m/%Y %H:%M:%S", &dateTime);

  return formattedDate;
}

#endif