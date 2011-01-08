LIBS = ../tinyxml/tinyxml.a -ldl

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
	MythXmlResponse.cpp \

include ../../../../Makefile.include

INCLUDES += -I../../../addons/include -I../../../FileSystem

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -g $(OBJS) $(LIBS) $(LIBDIRS) $(SILIB) -o $(LIB)
