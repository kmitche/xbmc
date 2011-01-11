LIB = libmythsql.a

SRCS = \
	MythSql.cpp \
	MythSqlResult.cpp \
	ScheduleCommands1254.cpp

include ../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

# Includes for the addon handles.
INCLUDES = -I../../../addons/include
