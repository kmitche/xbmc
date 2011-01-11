#pragma once
#include <vector>
#include <mysql/mysql.h>
#include "../StdString.h"
#include "SSchedule.h"

class IScheduleCommands
{
public:
  virtual int  GetNumberOfSchedules(MYSQL* conn) = 0;
  virtual bool GetAllSchedules(std::vector<SSchedule>& schedules, MYSQL* conn) = 0;
  virtual bool AddSchedule(const SSchedule& schedule, MYSQL* conn) = 0;
  virtual bool DeleteSchedule(const SSchedule& schedule, MYSQL* conn) = 0;
  virtual bool RenameSchedule(const SSchedule& schedule, const CStdString& newname, MYSQL* conn) = 0;
};


