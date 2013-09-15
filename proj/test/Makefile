# -*- mode: makefile -*-

# Author:   Huanghao@cellcom
# Date:     2005/09/29 17:31:11
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
STATIC_LIBS = $(RAD_HOME)/lib/radutil.a

# Dynamic linking libraries
LIBS = #-lws2_32

# Include directory searching path
INCS = -I$(RAD_HOME) \
       -I$(RAD_HOME)/radutil

# Executable output
BIN = runtest.exe
ARLIB =

# Intermedia objects
OBJS = \
radtest.o \
test_array.o \
test_common.o \
test_heaptrack.o \
test_main.o \
test_slist.o \
test_token.o \
test_util.o
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
radtest.o: radtest.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  radtest.h
test_array.o: test_array.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h radtest.h
test_common.o: test_common.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h radtest.h
test_heaptrack.o: test_heaptrack.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h radtest.h
test_main.o: test_main.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  radtest.h
test_slist.o: test_slist.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h radtest.h
test_token.o: test_token.cpp ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h radtest.h
test_util.o: test_util.cpp ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  radtest.h
# DEPS_END
