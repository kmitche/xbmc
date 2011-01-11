#include "MythSqlResult.h"

time_t MythSqlResult::ToDateTime(const CStdString& str)
{
   /*
   * YYYY-MM-DDTHH:MM:SS
   */
  if (str.size() != 19)
    return 0;

  tm time;
  time.tm_year  = atoi(str.Mid(0, 4)) - 1900;
  time.tm_mon   = atoi(str.Mid(5, 2)) - 1;
  time.tm_mday  = atoi(str.Mid(8, 2));
  time.tm_hour  = atoi(str.Mid(11, 2));
  time.tm_min   = atoi(str.Mid(14, 2));
  time.tm_sec   = atoi(str.Mid(17, 2));
  return mktime(&time);
}

time_t MythSqlResult::ToDateTime(const char* date, const char* time)
{
  CStdString str;
  str.Format("%sT%s", date, time);
  return MythSqlResult::ToDateTime(str);
}
