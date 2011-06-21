#include "MythSqlQuery.h"

#include <stdio.h>

CStdString MythSqlQuery::ToDate(const time_t& time)
{
  /*
   * YYYY-MM-DD
   */
  char buffer[12];
  strftime(buffer, 12, "%Y-%m-%d", localtime(&time));
  return buffer;
}

CStdString MythSqlQuery::ToTime(const time_t& time)
{
  /*
   * hh:mm:ss
   */
  char buffer[12];
  strftime(buffer, 12, "%H:%M:%S", localtime(&time));
  return buffer;
}
