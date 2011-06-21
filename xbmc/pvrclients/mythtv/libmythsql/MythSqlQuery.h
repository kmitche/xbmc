#pragma once

#include <time.h>

#include "utils/StdString.h"

class MythSqlQuery
{
  public:
    static CStdString ToDate(const time_t& time);
    static CStdString ToTime(const time_t& time);
};
