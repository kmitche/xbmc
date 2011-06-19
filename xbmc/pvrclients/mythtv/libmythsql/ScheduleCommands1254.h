#pragma once
#include "IScheduleCommands.h"

class ScheduleCommands1254 : public IScheduleCommands
{
public:
  ScheduleCommands1254();
  virtual ~ScheduleCommands1254();
  virtual int  GetNumberOfSchedules(MYSQL* conn);
  virtual bool GetAllSchedules(std::vector<MythSchedule>& schedules, MYSQL* conn);
  virtual bool AddSchedule(const MythSchedule& schedule, MYSQL* conn);
  virtual bool DeleteSchedule(const MythSchedule& schedule, MYSQL* conn);
  virtual bool RenameSchedule(const MythSchedule& schedule, const CStdString& newname, MYSQL* conn);
};
