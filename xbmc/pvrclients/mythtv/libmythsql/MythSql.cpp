#include "MythSql.h"

#include "ScheduleCommands1254.h"
#include "../client.h"

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
      XBMC->Log(LOG_DEBUG, "%s - Unknown schema version: %i", __FUNCTION__, m_schema_version);
      return false;
  }

  MYSQL * conn = GetConnection();
  if (conn == NULL)
    return false;


  CloseConnection(conn);
  XBMC->Log(LOG_DEBUG, "%s - Initialized: mysql://%s:%s@%s", __FUNCTION__,
            m_username.c_str(), m_password.c_str(), m_hostname.c_str());
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

MYSQL* MythSql::GetConnection()
{
  MYSQL * conn = mysql_init(NULL);
  if (conn == NULL)
  {
    XBMC->Log(LOG_ERROR, "%s - Error initialising mysql connection: mysql://%s:%s@%s. errno: %u, error: '%s'",
              __FUNCTION__, m_username.c_str(), m_password.c_str(), m_hostname.c_str(),
              mysql_errno(conn), mysql_error(conn));
    return NULL;
  }

  conn = mysql_real_connect(conn, m_hostname.c_str(), m_username.c_str(), m_password.c_str(),
         m_dbname.c_str(), 0, NULL, 0);
  if (conn == NULL)
  {
    XBMC->Log(LOG_ERROR, "%s - Error connecting to mysql server: mysql://%s:%s@%s. errno: %u, error: '%s'",
              __FUNCTION__, m_username.c_str(), m_password.c_str(), m_hostname.c_str(),
              mysql_errno(conn), mysql_error(conn));
    return NULL;
  }

  return conn;
}

void MythSql::CloseConnection(MYSQL* conn)
{
  if (conn)
  {
    mysql_close(conn);
    conn = NULL;
  }
}

int MythSql::GetNumberOfSchedules()
{
  MYSQL* conn = GetConnection();

  if (conn == NULL) {
    XBMC->Log(LOG_ERROR, "%s - NULL connection passed back from GetConnection", __FUNCTION__);
    return 0;
  }

  int ret = m_schedule_commands->GetNumberOfSchedules(conn);

  CloseConnection(conn);
  return ret;
}

bool MythSql::GetAllSchedules(std::vector<SSchedule>& schedules)
{
  MYSQL* conn = GetConnection();
  if (conn == NULL)
    return false;

  bool ret = m_schedule_commands->GetAllSchedules(schedules, conn);

  CloseConnection(conn);
  return ret;
}

bool MythSql::AddSchedule(const SSchedule& schedule)
{
  MYSQL* conn = GetConnection();
  if (conn == NULL)
    return false;

  bool ret = m_schedule_commands->AddSchedule(schedule, conn);

  CloseConnection(conn);
  return ret;
}

bool MythSql::DeleteSchedule(const SSchedule& schedule)
{
  MYSQL* conn = GetConnection();
  if (conn == NULL)
    return false;

  bool ret = m_schedule_commands->DeleteSchedule(schedule, conn);

  CloseConnection(conn);
  return ret;
}

bool MythSql::RenameSchedule(const SSchedule& schedule, const CStdString& newname)
{
  MYSQL* conn = GetConnection();
  if (conn == NULL)
    return false;

  bool ret = m_schedule_commands->RenameSchedule(schedule, newname, conn);

  CloseConnection(conn);
  return ret;
}
