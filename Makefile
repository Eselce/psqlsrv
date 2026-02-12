CC         = g++
#CC         = clang
CFLAGS     = $(CFLAGSDEB)
CFLAGSDEB  = -c $(WFLAGS) $(DEBFLAGS)
CFLAGSREL  = -c $(WFLAGS) $(RELFLAGS)
WFLAGS     = -Wall
DEBFLAGS   = -g
RELFLAGS   = -O2
LDFLAGS    = 
CPPNUMBER  = 20
CPPVERSION = c++${CPPNUMBER}
CPPLIBS    = $(LDFLAGS) $(PGLIBS)
#CPPLIBS    = $(LDFLAGS) $(PGLIBS) $(HTTPLIBS) $(HTTPSLIBS)
PGLIBS     = -L/usr/lib/x86_64-linux-gnu -lpq
HTTPSLIBS  = -lssl -lcrypto
CPPFLAGS   = 
#CPPFLAGS   = -I/usr/include/postgresql
CPPSTYLE   = -std=${CPPVERSION}
CPPOPTS    = ${CPPSTYLE} ${CPPFLAGS}
HTTPLIB    = ./cpp-httplib
HTTPLIB_H  = ./httplib.h
MAKEFILE   = ./Makefile
README     = ./README.md
LAUNCH     = ./.vscode/launch.json
TASKS      = ./.vscode/tasks.json
BIN        = ~/bin
DEFAULT    = ${server_exe}
all_files  = ${src_files} ${exe_files}
exe_files  = ${BIN}/${server_exe}
obj_files  = $(src_files:.cpp=.o)
hpp_files  = $(src_files:.cpp=.hpp)
src_files  = ${server_cpp}
server_exe = server
server_cpp = main.cpp httpsrv.cpp pgconn.cpp

.DEFAULT_GOAL := ${DEFAULT}

.MAIN:	${DEFAULT}

.PHONY:	default all clean cleanall cleanobj install deps attic

default:	${DEFAULT}

all:	cleanall install

clean:	cleanobj
	$(RM) ${DEFAULT}

cleanall:	clean
	$(RM) $(exe_files)

cleanobj:
	$(RM) $(obj_files)

install:	${all_files}

deps:
	mkdir -p $(HTTPLIB)
	curl -fsSL https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master/httplib.h -o $(HTTPLIB)/httplib.h
	curl -fsSL https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master/README.md -o $(HTTPLIB)/README.md
	diff -b $(HTTPLIB_H) $(HTTPLIB)

attic:	$(MAKEFILE) $(README) $(LAUNCH) $(TASKS) $(HTTPLIB_H) $(src_files) $(hpp_files)
	mkdir -p $@/
	cp -p $^ $@/

${exe_files}:	${server_exe}
	mv -Tfuv $< $@

.cpp.o:
	$(CC) $(CFLAGS) ${CPPOPTS} $< -o $@

${server_exe}:	${obj_files}
	$(CC) ${CPPSTYLE} $^ $(CPPLIBS) -o $@

