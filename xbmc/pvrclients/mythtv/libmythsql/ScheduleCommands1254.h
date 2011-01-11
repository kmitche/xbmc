#pragma once
#include "IScheduleCommands.h"

class ScheduleCommands1254 : public IScheduleCommands
{
public:
  ScheduleCommands1254();
  virtual ~ScheduleCommands1254();
  virtual int  GetNumberOfSchedules(MYSQL* conn);
  virtual bool GetAllSchedules(std::vector<SSchedule>& schedules, MYSQL* conn);
  virtual bool AddSchedule(const SSchedule& schedule, MYSQL* conn);
  virtual bool DeleteSchedule(const SSchedule& schedule, MYSQL* conn);
  virtual bool RenameSchedule(const SSchedule& schedule, const CStdString& newname, MYSQL* conn);
};
