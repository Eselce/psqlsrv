CC         := g++
#CC         := clang
WFLAGS     := -Wall
DEBFLAGS   := -g -D DEBUG
RELFLAGS   := -O2 -D NDEBUG
CPPNUMBER  := 20
SRCDIR     := src
INCLUDEDIR := include
VSCODEDIR  := .vscode
ATTICDIR   := attic
SAVEDIRS   := $(VSCODEDIR) $(INCLUDEDIR) $(SRCDIR)
ATTICDIRS  := $(SAVEDIRS:%=$(ATTICDIR)/%)
BUILDDIR   := bin
BINDIR     := ~/bin
MAKEFILE   := Makefile
README     := README.md
GITIGNORE  := .gitignore
LAUNCH     := $(VSCODEDIR)/launch.json
TASKS      := $(VSCODEDIR)/tasks.json
server_exe := server
server_cpp := main.cpp httpsrv.cpp pgconn.cpp
CPPVERSION := c++$(CPPNUMBER)
CPPSTYLE   := -std=$(CPPVERSION)
CFLAGSDEB  := -c $(WFLAGS) $(DEBFLAGS)
CFLAGSREL  := -c $(WFLAGS) $(RELFLAGS)
CFLAGS     := $(CFLAGSDEB)
LDFLAGS    := 
PGLIBS     := -L/usr/lib/x86_64-linux-gnu -lpq
HTTPLIBS   := 
HTTPSLIBS  := -lssl -lcrypto
CPPLIBS    := $(LDFLAGS) $(PGLIBS)
#CPPLIBS    := $(LDFLAGS) $(PGLIBS) $(HTTPLIBS) $(HTTPSLIBS)
CPPFLAGS   := 
#CPPFLAGS   := -I/usr/include/postgresql
CPPOPTS    := $(CPPSTYLE) $(CPPFLAGS)
HTTPLIBDIR := cpp-httplib
HTTPLIBDOC := README.md
HTTPLIB_H  := httplib.h
HTTPLIBSRC := https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master
HTTPLIBDAT := $(HTTPLIB_H) $(HTTPLIBDOC)
HTTPLIBALL := $(HTTPLIBDAT:%=$(HTTPLIBDIR)/%)
HTTPLIBINC := $(HTTPLIB_H:%=$(INCLUDEDIR)/%)
exe_file   := $(BUILDDIR)/$(server_exe)
bin_file   := $(BINDIR)/$(server_exe)
cpp_files  := $(server_cpp)
obj_files  := $(cpp_files:.cpp=.o)
hpp_files  := $(cpp_files:.cpp=.hpp)
inc_files  := $(obj_files:%.o=$(INCLUDEDIR)/%.hpp)
src_files  := $(obj_files:%.o=$(SRCDIR)/%.cpp)
bld_files  := $(obj_files:%.o=$(BUILDDIR)/%.o)
all_files  := $(bin_file)
sav_files  := $(MAKEFILE) $(README) $(GITIGNORE) $(LAUNCH) $(TASKS) $(HTTPLIBINC) $(src_files) $(inc_files)
att_files  := $(sav_files:%=$(ATTICDIR)/%)
DEFAULT    := $(exe_file)

.DEFAULT_GOAL := default

.MAIN:	default

.PHONY:	default all build clean cleanall cleanobj cleanbackup debug install installdebug run rundebug deps backup

default:	$(BUILDDIR) $(DEFAULT)

all:	deps install backup

build:	cleanall install

debug:	default

install:	default $(all_files)

installdebug:	debug $(all_files)

run:	install
	$(bin_file)

rundebug:	installdebug
	gdb $(bin_file)

backup:	cleanbackup $(ATTICDIR) $(att_files)

deps:	$(HTTPLIBDIR) $(HTTPLIBALL)
	-diff -b $(HTTPLIBINC) $(HTTPLIBDIR)/$(HTTPLIB_H)

clean:	cleanobj
	$(RM) $(exe_file)
	rmdir -p $(BUILDDIR)/

cleanall:	clean
	$(RM) $(bin_file)

cleanobj:	$(BUILDDIR)
	$(RM) $(bld_files)

cleanbackup:	$(ATTICDIR)
	$(RM) $(att_files)
	-rmdir $(ATTICDIRS)
	rmdir -p $(ATTICDIR)

$(BUILDDIR):
	mkdir -p $@/

$(HTTPLIBDIR):
	mkdir -p $@/

$(ATTICDIR):
	mkdir -p $@/

$(att_files):	$(ATTICDIR)/%:	%
	@mkdir -p $(dir $@)
	cp -p $< $@

$(HTTPLIBALL):	$(HTTPLIBDIR)/%:	% $(HTTPLIBDIR)
	curl -fsSL $(HTTPLIBSRC)/$< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CPPOPTS) $< -o $@

$(bin_file):	$(exe_file)
	#mv -Tfuv $< $@
	cp -p $< $@

$(exe_file):	$(bld_files)
	$(CC) $(CPPSTYLE) $^ $(CPPLIBS) -o $@

$(bld_files):	$(BUILDDIR)/%.o:	$(SRCDIR)/%.cpp $(INCLUDEDIR)/%.hpp
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) $(CPPOPTS) $< -o $@

$(BUILDDIR)/httpsrv.o:	$(HTTPLIBINC)

