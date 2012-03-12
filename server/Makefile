#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#

CXX		 = g++
CXXFLAGS	 = -ggdb  -Wall -W -Werror -pedantic-errors
CXXFLAGS	+= -Wmissing-braces -Wparentheses
# The following option cannot be used since some of the socket
# macros give warnings on "old-style-cast"
#CXXFLAGS	+= -Wold-style-cast

SRC	= $(wildcard *.cc)

.PHONY: all clean cleaner

all: server

# Create the library; ranlib is for Darwin and maybe other systems.
# Doesn't seem to do any damage on other systems.

server: server.o ../clientserver/connection.o ../clientserver/server.o
	$(CXX) -o $@ $^


clean:
	$(RM) *.o

cleaner: clean
	$(RM) server

%.d: %.cc
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

include $(SRC:.cc=.d)