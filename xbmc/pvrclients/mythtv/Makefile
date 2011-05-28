# See the README for copyright information and how to reach the author.
LIB = ../../../addons/pvr.mythtv/XBMC_MythTV.pvr

LIBS = libmythxml/libmythxml.a libmythsql/libmythsql.a -ldl

# How deep does the rabbit hole go for FileCurl?
# LIBS += ../../FileSystem/FileCurl.o -ldl

# DIRS are used for included Makefile "clean" target
DIRS = libmythxml libmythsql

SRCS = \
	client.cpp \
	MythXml.cpp

include ../../../Makefile.include

# Reset INCLUDES to just be the libraries needed for the addon.
INCLUDES = -I../../../addons -I../../addons/include

# Additional include for FileCurl (and it's associated dependencies)
INCLUDES += -I../..

# Additional include for tinyxml (until MythXml.cpp moves to the libcmyth directory)
INCLUDES += -I../../../lib/tinyXML

# Additional include for PlatformDefs.h from IFile.h.
INCLUDES += -I../../linux

# Need to create a shared library since FileCurl cannot be referenced as its "in" XBMC
$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $(OBJS) $(LIBS) -o $(LIB)

# "deps" ensures that the LIBS are always made by default. This, unfortunately, means LIB is
# built for each make but it doesn't take very long.
$(LIB): deps

deps:
	$(MAKE) -C libmythxml
	$(MAKE) -C libmythsql
