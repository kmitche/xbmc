#include "ScheduleCommands1254.h"

#include <mysql/mysql.h>

#include "MythSqlQuery.h"
#include "MythSqlResult.h"

#include "../client.h"
#include "../MythTypes.h"

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
  if (mysql_query(conn, "SELECT \
    r.recordid, \
    r.type, \
    r.inactive, \
    r.title, \
    r.subtitle, \
    r.description, \
    r.category, \
    r.storagegroup, \
    r.chanid, \
    c.channum, \
    r.startdate, \
    r.starttime, \
    r.enddate, \
    r.endtime, \
    r.startoffset, \
    r.endoffset, \
    r.recpriority \
    FROM record AS r INNER JOIN channel AS c ON \
      r.chanid = c.chanid"))
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
    sched.category      = row[6]; // category
    sched.storagegroup  = row[7]; // storagegroup
    sched.chanid        = atoi(row[8]); // chanid
    sched.channum       = atoi(row[9]); // channum
    sched.start         = MythSqlResult::ToDateTime(row[10], row[11]); // startdate, starttime
    sched.end           = MythSqlResult::ToDateTime(row[12], row[13]); // enddate, endtime
    sched.startoffset   = atoi(row[14]); // startoffset
    sched.endoffset     = atoi(row[15]); // endoffset
    sched.priority      = atoi(row[16]); // recpriority
    schedules.push_back(sched);
  }
  mysql_free_result(res);
  return true;
}

bool ScheduleCommands1254::AddSchedule(const MythSchedule& schedule, MYSQL* conn)
{
  /*
   * Table definition for the record table can be found at http://www.mythtv.org/wiki/Record_table
   */
  int recordid;
  {
    /*
     * First, get a new recordid (the primary key) by finding out what the existing maximum is.
     */
    CStdString query = "SELECT MAX(recordid) FROM record";
    if (mysql_query(conn, query))
    {
      XBMC->Log(LOG_ERROR, "%s - Error querying for maximum recordid. ERROR %u: %s",
                __FUNCTION__, mysql_errno(conn), mysql_error(conn));
      return false;
    }
    MYSQL_RES* res = mysql_use_result(conn);
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(res)) != NULL)
      recordid = atoi(row[0]);
    else
      recordid = 0; // TODO: Not sure what it means if we don't get a row back. Error?
    mysql_free_result(res);
  }

  {
    CStdString query;
    query.Format("INSERT INTO record (recordid, type, chanid, startime, startdate, endtime, enddate, \
                 title, description, startoffset, endoffset) VALUES (%i, %i, %i, %s, %s,%s, %s, %s, %s, %i, %i)",
                 recordid++, kWeekslotRecord, schedule.chanid,
                 MythSqlQuery::ToTime(schedule.start).c_str(), MythSqlQuery::ToDate(schedule.start).c_str(),
                 MythSqlQuery::ToTime(schedule.end).c_str(), MythSqlQuery::ToDate(schedule.end).c_str(),
                 schedule.title, schedule.description, schedule.startoffset, schedule.endoffset);
    if (mysql_query(conn, query))
    {
     XBMC->Log(LOG_ERROR, "%s - Error inserting new schedule. ERROR %u: %s",
               __FUNCTION__, mysql_errno(conn), mysql_error(conn));
     return false;
    }
  }
  return true;
}

bool ScheduleCommands1254::DeleteSchedule(const int recordid, MYSQL* conn)
{
  CStdString query = "DELETE FROM record WHERE recordid = " + recordid;
  if (mysql_query(conn, query))
  {
    XBMC->Log(LOG_ERROR, "%s - Error deleting schedule. ERROR %u: %s",
              __FUNCTION__, mysql_errno(conn), mysql_error(conn));
    return false;
  }
  return true;
}

bool ScheduleCommands1254::RenameSchedule(const MythSchedule& schedule, const CStdString& newname, MYSQL* conn)
{
  return false;
}
