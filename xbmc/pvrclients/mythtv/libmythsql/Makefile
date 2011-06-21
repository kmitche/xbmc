LIB = libmythsql.a

SRCS = \
	MythSql.cpp \
	MythSqlQuery.cpp \
	MythSqlResult.cpp \
	ScheduleCommands1254.cpp

include ../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

# Reset INCLUDES to just be the libraries needed for the addon.
INCLUDES = -I../../../../addons -I../../../addons/include

# Includes for utils/StdString
INCLUDES += -I../../..