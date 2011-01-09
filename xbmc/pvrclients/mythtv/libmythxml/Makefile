# See the README for copyright information and how to reach the author.
LIB = libmythxml.a

DIRS = tinyxml

SRCS = \
	GetBackendTimeCommand.cpp \
	GetBackendVersionCommand.cpp \
	GetChannelListCommand.cpp \
	GetDriveSpaceCommand.cpp \
	GetNumOfChannelsCommand.cpp \
	GetNumOfRecordingsCommand.cpp \
	GetProgramGuideCommand.cpp \
	GetRecordedCommand.cpp \
	MythXmlCommand.cpp \
	MythXmlParameters.cpp \
	MythXmlResponse.cpp \

include ../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

# Includes for StdString
INCLUDES = -I..

# Additional includes for the addon handles.
INCLUDES += -I../../../addons/include
	
# "deps" ensures that the LIBS are always made by default. This, unfortunately, means LIB is
# built for each make but it doesn't take very long.
$(LIB): deps

deps:
	$(MAKE) -C tinyxml
