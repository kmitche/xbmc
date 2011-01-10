# See the README for copyright information and how to reach the author.
LIB = libmythxml.a

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
	MythXmlResponse.cpp

include ../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

# Includes for the addon handles.
INCLUDES = -I../../../addons/include

# Use the XBMC tinyXML classes for the moment.
INCLUDES += -I../../../../guilib
