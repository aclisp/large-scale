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
INCS = -I$(RAD_HOME)

# Executable output
BIN =
ARLIB = $(RAD_HOME)/lib/libradutil.a

# Intermedia objects
OBJS = \
radarray.o \
radcommon.o \
radconffile.o \
raddebug.o \
radheaptrack.o \
radipc.o \
radlog.o \
radposix.o \
radslist.o \
radtoken.o \
radutil.o
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
radarray.o: radarray.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radcommon.o: radcommon.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radconffile.o: radconffile.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
raddebug.o: raddebug.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radheaptrack.o: radheaptrack.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radipc.o: radipc.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radlog.o: radlog.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radposix.o: radposix.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radslist.o: radslist.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radtoken.o: radtoken.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
radutil.o: radutil.c radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h
# DEPS_END
