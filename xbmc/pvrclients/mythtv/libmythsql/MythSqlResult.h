#pragma once

#include <mysql/mysql.h>

#include "utils/StdString.h"

class MythSqlResult
{
public:
	static time_t ToDateTime(const CStdString& str);
	static time_t ToDateTime(const char* date, const char* time);
};
