#include "ScheduleCommands1254.h"

#include <mysql/mysql.h>
#include "MythSqlResult.h"

ScheduleCommands1254::ScheduleCommands1254()
{
}

ScheduleCommands1254::~ScheduleCommands1254()
{
}

int ScheduleCommands1254::GetNumberOfSchedules(MYSQL* conn)
{
  if (!mysql_query(conn, "SELECT count(*) FROM record"))
  {
    // TODO log the error
    // fprintf(stderr, "%s\n", mysql_error(conn));
    return 0;
  }
  MYSQL_RES* res = mysql_use_result(conn);
  int num_schedules = 0;
  try
  {
    MYSQL_ROW row; // array of string const char**
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      num_schedules = atoi(row[0]);
    }
    mysql_free_result(res);
  }
  catch(...)
  {
    return 0;
  }
  return num_schedules;
}

bool ScheduleCommands1254::GetAllSchedules(std::vector<SSchedule>& schedules, MYSQL* conn)
{
  if (!mysql_query(conn, "SELECT recordid, type, inactive, title, storagegroup, chanid, startdate, starttime, enddate, endtime, recpriority, autoexpire FROM record"))
  {
    // TODO log the error
    // fprintf(stderr, "%s\n", mysql_error(conn));
    return false;
  }
  MYSQL_RES* res = mysql_use_result(conn);
  try
  {
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
      SSchedule sched;
      sched.m_id        = atoi(row[0]); // recordid
      sched.m_recType   = (RecordingTypes)atoi(row[1]); // type
      sched.m_active    = atoi(row[2]) == 0; // inactive
      sched.m_title     = row[3]; // title
      sched.m_directory = row[4]; // storagegroup
      sched.m_channel   = atoi(row[5]); // chanid
      sched.m_starttime = MythSqlResult::ToDateTime(row[6], row[7]); // startdate, starttime
      sched.m_endtime   = MythSqlResult::ToDateTime(row[8], row[9]); // enddate, endtime
      sched.m_priority  = atoi(row[10]); // recpriority
      // sched.m_lifetime = atoi(row[11]); // autoexpire
      schedules.push_back(sched);
    }
    mysql_free_result(res);
  }
  catch(...)
  {
    return false;
  }
  return true;
}

bool ScheduleCommands1254::AddSchedule(const SSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::DeleteSchedule(const SSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::RenameSchedule(const SSchedule& schedule, const CStdString& newname, MYSQL* conn)
{
  return false;
}
