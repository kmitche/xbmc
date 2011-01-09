# Modified version of the original tinyxml Makefile to correctly generate the library packaged
# with the addon.
LIB = libtinyxml.a

SRCS = \
	tinystr.cpp \
	tinyxml.cpp \
	tinyxmlerror.cpp \
	tinyxmlparser.cpp

include ../../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

INCLUDES = 