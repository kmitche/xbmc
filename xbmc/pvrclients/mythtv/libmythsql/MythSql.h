#pragma once

#include <mysql/mysql.h>
#include <vector>
#include "IScheduleCommands.h"

#define SCHEMA_VER_0_23_1 1254

class MythSql
{
public:
	MythSql(CStdString& hostname, CStdString& username, CStdString& password,
          CStdString& dbname, int schemaVersion);
	~MythSql();
	bool Init();
	void Destroy();
	int  GetNumberOfSchedules();
	bool GetAllSchedules(std::vector<SSchedule>& schedules);
	bool AddSchedule(const SSchedule& schedule);
	bool DeleteSchedule(const SSchedule& schedule);
	bool RenameSchedule(const SSchedule& schedule, const CStdString& newname);

private:
	CStdString m_hostname;
	CStdString m_username; 
	CStdString m_password; 
	CStdString m_dbname;
	int        m_schema_version;

	bool GetConnection(MYSQL*);
	void CloseConnection(MYSQL* conn);

	IScheduleCommands* m_schedule_commands;
};
