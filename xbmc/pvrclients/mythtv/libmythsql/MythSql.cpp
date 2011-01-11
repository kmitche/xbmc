#include "MythSql.h"

#include "ScheduleCommands1254.h"

#define DEFAULT_MYSQL_PORT 3306

MythSql::MythSql(CStdString& hostname, CStdString& username, CStdString& password,
                 CStdString& dbname, int schemaVersion)
{
  m_hostname = hostname;
  m_username = username;
  m_password = password;
  m_dbname = dbname;
  m_schema_version = schemaVersion;
}

MythSql::~MythSql()
{
  Destroy();
}

bool MythSql::Init()
{
  /* Check that there is a valid set of SQL statements prior to connecting. */
  switch (m_schema_version)
  {
    case SCHEMA_VER_0_23_1:
      m_schedule_commands = new ScheduleCommands1254();
      break;
    default:
      return false;
  }

  MYSQL* conn;
  if (!GetConnection(conn))
    return false;

  CloseConnection(conn);
  return true;
}

void MythSql::Destroy()
{
  if (m_schedule_commands)
  {
    delete m_schedule_commands;
    m_schedule_commands = NULL;
  }
}

bool MythSql::GetConnection(MYSQL* conn)
{
  // TODO: Get from connection pool.
  conn = mysql_init(NULL);
  if (conn == NULL)
    return false;

  // TODO: Advanced configuration needed for the MySQL port?
  return mysql_real_connect(conn, m_hostname.c_str(), m_username.c_str(), m_password.c_str(),
         m_dbname.c_str(), DEFAULT_MYSQL_PORT, NULL, 0);
}

void MythSql::CloseConnection(MYSQL* conn)
{
  // TODO: Return to connection pool.
  if (conn)
  {
    try
    {
      mysql_close(conn);
      conn = NULL;
    }
    catch(...)
    {
      // TODO: Log
    }
  }
}

int MythSql::GetNumberOfSchedules()
{
  MYSQL* conn;
  if (!GetConnection(conn))
    return 0;

  int ret;
  try
  {
    ret = m_schedule_commands->GetNumberOfSchedules(conn);
  }
  catch(...)
  {
    // TODO: Log
    ret = 0;
  }
  CloseConnection(conn);
  return ret;
}

bool MythSql::GetAllSchedules(std::vector<SSchedule>& schedules)
{
  MYSQL* conn;
  if (!GetConnection(conn))
    return false;

  bool ret;
  try
  {
    ret = m_schedule_commands->GetAllSchedules(schedules, conn);
  }
  catch (...)
  {
    // TODO: Log
    ret = false;
  }
  CloseConnection(conn);
  return ret;
}

bool MythSql::AddSchedule(const SSchedule& schedule)
{
  MYSQL* conn;
  if (!GetConnection(conn))
    return false;

  bool ret;
  try
  {
    ret = m_schedule_commands->AddSchedule(schedule, conn);
  }
  catch (...)
  {
    // TODO: Log
    ret = false;
  }
  CloseConnection(conn);
  return ret;
}

bool MythSql::DeleteSchedule(const SSchedule& schedule)
{
  MYSQL* conn;
  if (!GetConnection(conn))
    return false;

  bool ret;
  try
  {
    ret = m_schedule_commands->DeleteSchedule(schedule, conn);
  }
  catch (...)
  {
    // TODO: Log
    ret = false;
  }
  CloseConnection(conn);
  return ret;
}

bool MythSql::RenameSchedule(const SSchedule& schedule, const CStdString& newname)
{
  MYSQL* conn;
  if (!GetConnection(conn))
    return false;

  bool ret;
  try
  {
    // TODO: Log
    ret = m_schedule_commands->RenameSchedule(schedule, newname, conn);
  }
  catch (...)
  {
    ret = false;
  }
  CloseConnection(conn);
  return ret;
}
