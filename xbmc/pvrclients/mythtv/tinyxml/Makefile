# Modified version of the original tinyxml Makefile to correctly generate the library packaged
# with the addon.

LIB = tinyxml.a

SRCS = \
	tinyxml.cpp \
	tinyxmlparser.cpp \
	tinyxmlerror.cpp \
	tinystr.cpp

include ../../../../Makefile.include

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -g $(OBJS) $(LIBS) $(LIBDIRS) $(SILIB) -o $(LIB)
