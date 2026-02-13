CXX        := g++
#CXX        := clang
WFLAGS     := -Wall -Wextra
DEBFLAGS   := -g -D_DEBUG
RELFLAGS   := -O2 -DNDEBUG
CPPNUMBER  := 20
SRCDIR     := src
INCLUDEDIR := include
VSCODEDIR  := .vscode
ATTICDIR   := attic
SAVEDIRS   := $(VSCODEDIR) $(INCLUDEDIR) $(SRCDIR)
ATTICDIRS  := $(SAVEDIRS:%=$(ATTICDIR)/%)
BUILDSUFF  :=
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
CFLAGSDEB  := $(WFLAGS) $(DEBFLAGS)
CFLAGSREL  := $(WFLAGS) $(RELFLAGS)

ifeq ($(DEBUG),1)
CXXCONFIG  := $(CFLAGSDEB)
BUILDSUFF  := D
BUILDDIR   := debug
else
ifeq ($(DEBUG),0)
CXXCONFIG  := $(CFLAGSREL)
BUILDDIR   := release
else
BUILDSUFF  := _
endif
endif

exe_file   := $(BUILDDIR)/$(server_exe)$(BUILDSUFF)
bin_file   := $(BINDIR)/$(server_exe)$(BUILDSUFF)

LDFLAGS    := 
PGLIBS     := -L/usr/lib/x86_64-linux-gnu -lpq
HTTPLIBS   := 
HTTPSLIBS  := -lssl -lcrypto
CXXLIBS    := $(LDFLAGS) $(PGLIBS)
#CXXLIBS    := $(LDFLAGS) $(PGLIBS) $(HTTPLIBS) $(HTTPSLIBS)
CFLAGS     := 
CPPFLAGS   := 
#CPPFLAGS   := -I/usr/include/postgresql
CPPOPTS    := $(CPPSTYLE)
#CPPOPTS    := $(CPPSTYLE) $(CFLAGS) $(CPPFLAGS)
LXXFLAGS   := $(CPPOPTS) $(CXXLIBS)
CXXFLAGS   := $(CPPOPTS) $(CXXCONFIG) -I $(INCLUDEDIR)

HTTPLIBDIR := cpp-httplib
HTTPLIBDOC := README.md
HTTPLIB_H  := httplib.h
HTTPLIBSRC := https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master
HTTPLIBDAT := $(HTTPLIB_H) $(HTTPLIBDOC)
HTTPLIBALL := $(HTTPLIBDAT:%=$(HTTPLIBDIR)/%)
HTTPLIBINC := $(HTTPLIB_H:%=$(INCLUDEDIR)/%)

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

.PHONY:	default all build buildall clean cleanall cleanobj cleanbackup cleandeps install installdebug run rundebug deps backup

default:	$(BUILDDIR) $(DEFAULT)

all:	deps install backup

build:	cleanall install

buildall:	deps cleanall install backup

install:	default $(all_files)

installdebug:	default $(all_files)

run:	install
	$(bin_file)

rundebug:	installdebug
	gdb $(bin_file)

backup:	cleanbackup $(ATTICDIR) $(att_files)

deps:	cleandeps $(HTTPLIBDIR) $(HTTPLIBALL)
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

cleandeps:	$(HTTPLIBDIR)
	$(RM) $(HTTPLIBALL)
	rmdir -p $(HTTPLIBDIR)

$(BUILDDIR):
	mkdir -p $@/

$(HTTPLIBDIR):
	mkdir -p $@/

$(ATTICDIR):
	mkdir -p $@/

$(att_files):	$(ATTICDIR)/%:	%
	@mkdir -p $(dir $@)
	cp -p $< $@

$(HTTPLIBALL):	$(HTTPLIBDIR)/%:	$(HTTPLIBDIR)
	@mkdir -p $(dir $@)
	curl -fsSL $(HTTPLIBSRC)/$(notdir $@) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(bin_file):	$(exe_file)
	#mv -Tfuv $< $@
	cp -p $< $@

$(exe_file):	$(bld_files)
	$(CXX) $^ $(LXXFLAGS) -o $@

$(bld_files):	$(BUILDDIR)/%.o:	$(SRCDIR)/%.cpp $(INCLUDEDIR)/%.hpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/httpsrv.o:	$(HTTPLIBINC)

