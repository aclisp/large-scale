# -*- mode: makefile -*-

# Author:   Homer Huang
# Date:     2011/08/31 15:10:45
# Revision: 0.0.1
# Project:
# Usage:

# Properties
RAD_HOME = ../proj

# Compiler
CC = gcc
CXX = g++
AR = ar

# Static linking libraries
STATIC_LIBS =

# Dynamic linking libraries
LIBS = -lrt

# Include directory searching path
INCS = -I$(RAD_HOME)

# Executable output
BIN = prog
ARLIB =

# Intermedia objects
OBJS = \
Class.o \
InvokerThread.o \
LeakyBucketCounter.o \
Message.o \
MessagePool.o \
Object.o \
ObjectPool.o \
PooledObject.o \
RootThread.o \
SingletonObject.o \
Thread.o \
TlvMessage.o \
Trace.o \
Xsyslib.o \
program_entry.o \
udpService.o
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
Class.o: Class.cpp Class.h Object.h PooledObject.h ObjectPool.h
InvokerThread.o: InvokerThread.cpp InvokerThread.h Thread.h Object.h \
  SingletonObject.h Xsyslib.h Trace.h LeakyBucketCounter.h
LeakyBucketCounter.o: LeakyBucketCounter.cpp LeakyBucketCounter.h \
  Object.h Thread.h SingletonObject.h Xsyslib.h Trace.h
Message.o: Message.cpp Message.h PooledObject.h Object.h MessagePool.h \
  ObjectPool.h SingletonObject.h Xsyslib.h Trace.h
MessagePool.o: MessagePool.cpp MessagePool.h ObjectPool.h Object.h \
  SingletonObject.h Xsyslib.h Trace.h
Object.o: Object.cpp Object.h Class.h
ObjectPool.o: ObjectPool.cpp ObjectPool.h Object.h PooledObject.h \
  Xsyslib.h Trace.h
PooledObject.o: PooledObject.cpp PooledObject.h Object.h
RootThread.o: RootThread.cpp RootThread.h Thread.h Object.h \
  SingletonObject.h Xsyslib.h Trace.h
SingletonObject.o: SingletonObject.cpp SingletonObject.h Xsyslib.h \
  Trace.h
Thread.o: Thread.cpp Thread.h Object.h SingletonObject.h Xsyslib.h \
  Trace.h
TlvMessage.o: TlvMessage.cpp TlvMessage.h Message.h PooledObject.h \
  Object.h
Trace.o: Trace.cpp Trace.h Thread.h Object.h SingletonObject.h Xsyslib.h
Xsyslib.o: Xsyslib.cpp Xsyslib.h Trace.h
program_entry.o: program_entry.cpp Xsyslib.h Thread.h Object.h \
  SingletonObject.h Trace.h RootThread.h InvokerThread.h \
  LeakyBucketCounter.h MessagePool.h ObjectPool.h TlvMessage.h Message.h \
  PooledObject.h
udpService.o: udpService.cpp udpService.h Thread.h Object.h \
  SingletonObject.h Xsyslib.h Trace.h
# DEPS_END
