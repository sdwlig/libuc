# Contents of this file are not meant to be changed by the user.
# Instead, change the variables defined in config.inc,
# Or run the provided configure script to set the appropriate
# variables in the environment.

include config.inc

ifeq ($(CPU),CORE2)
CPUFLAGS= -mtune=core2  -march=core2 -m64
endif

ifeq ($(CPU),PRESCOTT)
CPUFLAGS= -mtune=prescott  -march=prescott -m32
endif

ifeq ($(CPU),PENTIUMPRO)
CPUFLAGS= -mtune=pentiumpro -march=pentiumpro -m32
endif

ifeq ($(CPU),G5)
CPUGLAGS= -march=G5 -mtune=G5  -mpowerpc64 -mpowerpc-gpopt -faltivec
endif

ifeq ($(CPU),G4)
CPUFLAGS= -march=G4 -mtune=G4 -faltivec
endif

ifeq ($(CPU),GENERIC)
CPUFLAGS=
endif

#Debugging/Optimization
ifeq ($(DEBUG),YES)
DFLAGS=-g
else
DFLAGS=-O3
endif

#Optional Library Defines
ifeq ($(MYSQL),YES)
COPTFLAGS += $(MYSQL_CFLAGS)
LOPTFLAGS += $(MYSQL_LIBS)
endif

#Optional Library Defines
ifeq ($(CURL),YES)
COPTFLAGS += $(CURL_CFLAGS)
LOPTFLAGS += $(CURL_LIBS)
endif

#Optional Library Defines
ifeq ($(SQLITE),YES)
COPTFLAGS += $(SQLITE_CFLAGS)
LOPTFLAGS += $(SQLITE_LIBS)
endif

ifeq ($(CPILER), CLANG)
CC = cc
CXX = c++
endif

ifeq ($(CPILER), GCC)
CC = gcc
CXX = g++
endif

#Compilier defines
CFLAGS = -Wall $(CPUFLAGS) $(OSFLAGS) $(DFLAGS) $(COPTFLAGS)
LINKXX = $(CXX) $(CPUFLAGS) $(OSFLAGS) $(DFLAGS) $(LOPTFLAGS)
CXXFLAGS = $(CFLAGS)
STATICLIB=ar -r
