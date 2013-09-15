# -*- mode: makefile -*-

# Author:   Huanghao@cellcom
# Date:     2005/09/29 16:58:33
# Revision: 0.0.1
# Project:
# Usage:

# Properties
RAD_HOME = ..

# Compiler
CC = gcc
CXX = g++
AR = ar

# Static linking libraries
STATIC_LIBS =

# Dynamic linking libraries
LIBS =

# Include directory searching path
INCS = -I$(RAD_HOME) \
       -I$(RAD_HOME)/radutil \
       -I$(RAD_HOME)/radsock

# Executable output
BIN =
ARLIB = $(RAD_HOME)/lib/radfwk.a

# Intermedia objects
OBJS = \
ConfMap.o \
Config.o \
Event_Handler.o \
Reactor.o \
radalloc.o
# OBJS_END

# Global defined macros
DEFS =

# Compiling flags
CFLAGS = $(INCS) $(DEFS) -Wall -g
ARFLAGS = cr

# Primary entry
all: $(ARLIB) $(BIN)

# Print all objects
po:
	@-ls -1 *.c *.cpp | sed "$$ ! s@\.[^\.]*\$$@.o \\\\@ ; $$ s@\.[^\.]*\$$@.o@"

# Print all dependencies
pd:
	@-$(CXX) -MM *.c *.cpp $(INCS) $(DEFS)

# Clean target
clean:
	rm -f $(BIN) $(ARLIB) $(OBJS)

# Link target
$(BIN): $(OBJS)
	$(CXX) $^ $(STATIC_LIBS) -o $@ $(LIBS) $(CFLAGS)
$(ARLIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Suffix rules
.cpp.o:
	$(CXX) -c $< -o $@ $(CFLAGS)
.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

# Dependencies
# DEPS_BEGIN
ConfMap.o: ConfMap.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ConfMap.h ../radfwk/noncopyable.hpp functors.hpp ListMap.hpp
Config.o: Config.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  Config.h ../radfwk/noncopyable.hpp ConfMap.h functors.hpp ListMap.hpp
Event_Handler.o: Event_Handler.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h Event_Handler.h ../radfwk/radalloc.hpp
Reactor.o: Reactor.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h Event_Handler.h ../radfwk/radalloc.hpp Reactor.h \
  ../radfwk/noncopyable.hpp
radalloc.o: radalloc.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  radalloc.hpp
# DEPS_END
