#include "ScheduleCommands1254.h"

#include <mysql/mysql.h>

#include "MythSqlResult.h"

#include "../client.h"

ScheduleCommands1254::ScheduleCommands1254()
{
}

ScheduleCommands1254::~ScheduleCommands1254()
{
}

int ScheduleCommands1254::GetNumberOfSchedules(MYSQL* conn)
{
  if (mysql_query(conn, "SELECT count(*) FROM record")) // 0 is successful
  {
    XBMC->Log(LOG_ERROR, "%s - Error querying for number of schedules. ERROR %u: %s",
              __FUNCTION__, mysql_errno(conn), mysql_error(conn));
    return 0;
  }

  MYSQL_RES* res = mysql_use_result(conn);

  int num_schedules = 0;
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    num_schedules = atoi(row[0]);
  }
  mysql_free_result(res);
  return num_schedules;
}

bool ScheduleCommands1254::GetAllSchedules(std::vector<MythSchedule>& schedules, MYSQL* conn)
{
  if (mysql_query(conn, "SELECT r.recordid, r.type, r.inactive, r.title, r.subtitle, r.description, r.storagegroup, r.chanid, c.channum, r.startdate, r.starttime, r.enddate, r.endtime, r.startoffset, r.endoffset, r.recpriority FROM record AS r INNER JOIN channel AS c ON r.chanid = c.chanid"))
  {
    XBMC->Log(LOG_ERROR, "%s - Error querying for all schedules. ERROR %u: %s",
              __FUNCTION__, mysql_errno(conn), mysql_error(conn));
    return false;
  }
  MYSQL_RES* res = mysql_use_result(conn);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res)) != NULL)
  {
    MythSchedule sched;
    sched.recordid      = atoi(row[0]); // recordid
    sched.type          = (MythScheduleType)atoi(row[1]); // type. TODO: safe conversion to ENUM
    sched.inactive      = atoi(row[2]) != 0; // inactive
    sched.title         = row[3]; // title
    sched.subtitle      = row[4]; // subtitle
    sched.description   = row[5]; // description
    sched.storagegroup  = row[6]; // storagegroup
    sched.chanid        = atoi(row[7]); // chanid
    sched.channum       = atoi(row[8]); // channum
    sched.start         = MythSqlResult::ToDateTime(row[9], row[10]); // startdate, starttime
    sched.end           = MythSqlResult::ToDateTime(row[11], row[12]); // enddate, endtime
    sched.startoffset   = atoi(row[13]); // startoffset
    sched.endoffset     = atoi(row[14]); // endoffset
    sched.priority      = atoi(row[15]); // recpriority
    schedules.push_back(sched);
  }
  mysql_free_result(res);
  return true;
}

bool ScheduleCommands1254::AddSchedule(const MythSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::DeleteSchedule(const MythSchedule& schedule, MYSQL* conn)
{
  return false;
}

bool ScheduleCommands1254::RenameSchedule(const MythSchedule& schedule, const CStdString& newname, MYSQL* conn)
{
  return false;
}
