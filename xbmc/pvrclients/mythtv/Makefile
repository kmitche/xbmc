# See the README for copyright information and how to reach the author.
LIB = ../../../addons/pvr.mythtv/XBMC_MythTV.pvr

LIBS = libmythxml/libmythxml.a -ldl

# How deep does the rabbit hole go for FileCurl?
# LIBS += ../../FileSystem/FileCurl.o -ldl

# DIRS are used for included Makefile "clean" target
DIRS = libmythxml

SRCS = \
	client.cpp \
	MythXml.cpp

include ../Makefile.include

# Additional include for PlatformDefs.h from IFile.h.
INCLUDES += -I../../linux

# "deps" ensures that the LIBS are always made by default. This, unfortunately, means LIB is
# built for each make but it doesn't take very long.
$(LIB): deps

deps:
	$(MAKE) -C libmythxml
