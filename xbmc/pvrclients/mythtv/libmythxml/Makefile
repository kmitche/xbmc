INCLUDES = -I../../../addons/include -I../../../FileSystem

LIB = libmythxml.a

SRCS := \
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

GetBackendTimeCommand.o : GetBackendTimeCommand.h
GetBackendVersionCommand.o : GetBackendVersionCommand.h
GetChannelListCommand.o : GetChannelListCommand.h
GetDriveSpaceCommand.o : GetDriveSpaceCommand.h
GetNumOfChannelsCommand.o : GetNumOfChannelsCommand.h
GetNumOfRecordingsCommand.o : GetNumOfRecordingsCommand.h
GetProgramGuideCommand.o : GetProgramGuideCommand.h
GetRecordedCommand.o : GetRecordedCommand.h
MythXmlCommand.o : 
MythXmlParameters.o : 
MythXmlResponse.o : 

$(OBJS) : \
  SChannel.h \
  SEpg.h \
  SRecordingInfo.h \
  MythXmlCommand.h \
  MythXmlParameters.h \
  MythXmlResponse.h

#Remaining can be used once the dependencies on upstream XBMC classes (CUrl and StdString) have been sorted out.
#OBJS := $(addsuffix .o,$(basename ${SRCS}))

#$(LIB): $(OBJS)

# Override the default Make rule to allow for INCLUDES
#%.o: %.cpp
#	$(COMPILE.cpp) $(INCLUDES) $(OUTPUT_OPTION) $<

#clean:
#	$(RM) $(OBJS) $(LIB)
