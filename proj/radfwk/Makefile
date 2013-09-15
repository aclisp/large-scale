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
STATIC_LIBS = $(RAD_HOME)/lib/radutil.a \
              $(RAD_HOME)/lib/radsock.a

# Dynamic linking libraries
#LIBS = -shared
LIBS =

# Include directory searching path
INCS = -I$(RAD_HOME) \
       -I$(RAD_HOME)/radutil \
       -I$(RAD_HOME)/radsock \
       -I$(RAD_HOME)/radsql

# Executable output
#BIN = $(RAD_HOME)/lib/libradfwk.so
BIN =
ARLIB = $(RAD_HOME)/lib/radfwk.a

# Intermedia objects
OBJS = \
RAD_AppConf.o \
RAD_ConfHelper.o \
RAD_Cron.o \
RAD_DBPool.o \
RAD_Global.o \
RAD_Lock.o \
RAD_Object.o \
RAD_Parameters.o \
RAD_Reactor.o \
RAD_Thread.o \
RAD_Util.o
# OBJS_END

# Global defined macros
DEFS = -DRADFWK_STATIC

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
RAD_AppConf.o: RAD_AppConf.cpp RAD_AppConf.h ../radfwk/RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h RAD_ConfHelper.h RAD_Util.h
RAD_ConfHelper.o: RAD_ConfHelper.cpp RAD_ConfHelper.h \
  ../radfwk/RAD_Object.h ../radfwk/RAD_Global.h ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h ../radsock/radsock.h ../radsql/radsql.h RAD_Util.h
RAD_Cron.o: RAD_Cron.cpp RAD_Cron.h ../radfwk/RAD_Thread.h \
  ../radfwk/RAD_Object.h ../radfwk/RAD_Global.h ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h ../radsock/radsock.h ../radsql/radsql.h \
  ../radfwk/RAD_Lock.h
RAD_DBPool.o: RAD_DBPool.cpp RAD_DBPool.h RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h
RAD_Global.o: RAD_Global.cpp RAD_Global.h ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h ../radsock/radsock.h ../radsql/radsql.h \
  RAD_Object.h ../radfwk/RAD_Global.h RAD_Cron.h ../radfwk/RAD_Thread.h \
  ../radfwk/RAD_Object.h ../radfwk/RAD_Lock.h
RAD_Lock.o: RAD_Lock.cpp RAD_Lock.h ../radfwk/RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h
RAD_Object.o: RAD_Object.cpp RAD_Object.h ../radfwk/RAD_Global.h \
  ../radutil/radutil.h ../radutil/radplatform.h ../radutil/radtypes.h \
  ../radutil/raddebug.h ../radutil/radposix.h ../radutil/radmacros.h \
  ../radutil/radcommon.h ../radutil/radlog.h ../radutil/radslist.h \
  ../radutil/radheaptrack.h ../radutil/radarray.h ../radutil/radtoken.h \
  ../radutil/radconffile.h ../radutil/radipc.h ../radsock/radsock.h \
  ../radsql/radsql.h RAD_Parameters.h ../radfwk/RAD_Object.h
RAD_Parameters.o: RAD_Parameters.cpp RAD_Parameters.h \
  ../radfwk/RAD_Object.h ../radfwk/RAD_Global.h ../radutil/radutil.h \
  ../radutil/radplatform.h ../radutil/radtypes.h ../radutil/raddebug.h \
  ../radutil/radposix.h ../radutil/radmacros.h ../radutil/radcommon.h \
  ../radutil/radlog.h ../radutil/radslist.h ../radutil/radheaptrack.h \
  ../radutil/radarray.h ../radutil/radtoken.h ../radutil/radconffile.h \
  ../radutil/radipc.h ../radsock/radsock.h ../radsql/radsql.h
RAD_Reactor.o: RAD_Reactor.cpp RAD_Reactor.h ../radfwk/RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h RAD_Thread.h \
  ../radfwk/RAD_Lock.h
RAD_Thread.o: RAD_Thread.cpp RAD_Thread.h ../radfwk/RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h ../radfwk/RAD_Lock.h RAD_Util.h
RAD_Util.o: RAD_Util.cpp RAD_Util.h ../radfwk/RAD_Object.h \
  ../radfwk/RAD_Global.h ../radutil/radutil.h ../radutil/radplatform.h \
  ../radutil/radtypes.h ../radutil/raddebug.h ../radutil/radposix.h \
  ../radutil/radmacros.h ../radutil/radcommon.h ../radutil/radlog.h \
  ../radutil/radslist.h ../radutil/radheaptrack.h ../radutil/radarray.h \
  ../radutil/radtoken.h ../radutil/radconffile.h ../radutil/radipc.h \
  ../radsock/radsock.h ../radsql/radsql.h
# DEPS_END
