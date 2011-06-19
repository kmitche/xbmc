#pragma once
#include <vector>
#include <mysql/mysql.h>
#include "utils/StdString.h"
#include "../MythTypes.h"

class IScheduleCommands
{
public:
  virtual int  GetNumberOfSchedules(MYSQL* conn) = 0;
  virtual bool GetAllSchedules(std::vector<MythSchedule>& schedules, MYSQL* conn) = 0;
  virtual bool AddSchedule(const MythSchedule& schedule, MYSQL* conn) = 0;
  virtual bool DeleteSchedule(const MythSchedule& schedule, MYSQL* conn) = 0;
  virtual bool RenameSchedule(const MythSchedule& schedule, const CStdString& newname, MYSQL* conn) = 0;
};
