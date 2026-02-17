CXX        := g++
#CXX        := clang
WFLAGS     := -Wall -Wextra
DEBFLAGS   := -g -D_DEBUG
RELFLAGS   := -O2 -DNDEBUG
CPPNUMBER  := 20
SRCDIR     := src
INCLUDEDIR := include
TESTDIR    := test
VSCODEDIR  := .vscode
HTTPLIBDIR := cpp-httplib
CATCH2DIR  := $(TESTDIR)/catch2
ATTICDIR   := attic
SAVEDIRS   := $(VSCODEDIR) $(INCLUDEDIR) $(SRCDIR) $(CATCH2DIR) $(TESTDIR)
ATTICDIRS  := $(SAVEDIRS:%=$(ATTICDIR)/%)
BUILDSUFF  :=
BUILDDIR   := bin
BINDIR     := ~/bin
MAKEFILE   := Makefile
README     := README.md
GITIGNORE  := .gitignore
LAUNCH     := $(VSCODEDIR)/launch.json
TASKS      := $(VSCODEDIR)/tasks.json

# server source files
server_exe := server
server_cpp := main.cpp \
	      nicesvc.cpp \
	      httpsrv.cpp \
	      dbconn.cpp \
	      dbparam.cpp \
	      dbrecset.cpp \
	      dbanswer.cpp \
	      dbstmt.cpp \
	      pgconn.cpp \
	      pgparam.cpp \
	      pgrecset.cpp \
	      pganswer.cpp \
	      pgstmt.cpp \
# EOL server source files

# test source files
test_exe   := tests
test_cpp   := test_dummy.cpp \
# EOL test source files

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

exe_test   := $(BUILDDIR)/$(test_exe)$(BUILDSUFF)
bin_test   := $(BINDIR)/$(test_exe)$(BUILDSUFF)

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

HTTPLIBDOC := README.md
HTTPLIBLIC := LICENSE
HTTPLIB_H  := httplib.h
HTTPLIBSRC := https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master
HTTPLIBDAT := $(HTTPLIB_H) $(HTTPLIBDOC) $(HTTPLIBLIC)
HTTPLIBALL := $(HTTPLIBDAT:%=$(HTTPLIBDIR)/%)
HTTPLIBINC := $(HTTPLIB_H:%=$(INCLUDEDIR)/%)

CATCH2_H   := catch.hpp
CATCH2SRC  := https://raw.githubusercontent.com/catchorg/Catch2/v2.13.10/single_include/catch2
CATCH2DAT  := $(CATCH2_H)
CATCH2ALL  := $(CATCH2DAT:%=$(CATCH2DIR)/%)
CATCH2INC  := $(CATCH2_H:%=$(CATCH2DIR)/%)

cpp_files  := $(server_cpp)
obj_files  := $(cpp_files:.cpp=.o)
hpp_files  := $(cpp_files:.cpp=.hpp)
inc_files  := $(obj_files:%.o=$(INCLUDEDIR)/%.hpp)
src_files  := $(obj_files:%.o=$(SRCDIR)/%.cpp)
bld_files  := $(obj_files:%.o=$(BUILDDIR)/%.o)
all_files  := $(bin_file)
cpp_tests  := $(test_cpp)
obj_tests  := $(cpp_tests:.cpp=.o)
hpp_tests  := $(cpp_tests:.cpp=.hpp)
inc_tests  := $(obj_tests:%.o=$(TESTDIR)/%.hpp)
src_tests  := $(obj_tests:%.o=$(TESTDIR)/%.cpp)
bld_tests  := $(obj_tests:%.o=$(BUILDDIR)/%.o)
all_tests  := $(bin_test)
sav_files  := $(MAKEFILE) $(README) $(GITIGNORE) $(LAUNCH) $(TASKS) $(HTTPLIBINC) $(CATCH2INC) $(src_files) $(inc_files) $(src_tests) $(inc_tests)
att_files  := $(sav_files:%=$(ATTICDIR)/%)
DEFAULT    := $(exe_file)

.DEFAULT_GOAL := default

.MAIN:	default

.PHONY:	default all build buildall clean cleanall cleanobj cleanbackup cleantest cleantests cleandeps cleanhttplib cleancatch2 builddebug buildrelease buildbin installdebug installrelease installbin configs configsbuild configsall install installdebug run rundebug testrun testrundebug tests testsdebug deps httplib catch2 backup

default:	$(BUILDDIR) $(DEFAULT)

all:	deps install backup

build:	cleanall install tests

buildall:	deps cleanall install tests backup

builddebug:
	$(MAKE) DEBUG=1 build

buildrelease:
	$(MAKE) DEBUG=0 build

buildbin:
	$(MAKE) DEBUG=2 build

installdebug:
	$(MAKE) DEBUG=1 build

installrelease:
	$(MAKE) DEBUG=0 build

installbin:
	$(MAKE) DEBUG=2 build

configs:	installdebug installrelease installbin

configsbuild:	builddebug buildrelease buildbin

configsall:	deps configsbuild backup

install:	default $(all_files)

installdebug:	default $(all_files)

run:	install
	$(bin_file)

rundebug:	installdebug
	gdb $(bin_file)

backup:	cleanbackup $(ATTICDIR) $(att_files)

tests:	$(CATCH2INC) $(all_tests)

testsdebug:	$(CATCH2INC) $(all_tests)

testrun:	tests
	$(bin_test)

testrundebug:	testsdebug
	gdb $(bin_test)

deps:	httplib $(CATCH2INC)

httplib:	cleanhttplib $(HTTPLIBDIR) $(HTTPLIBALL)
	-diff -b $(HTTPLIBINC) $(HTTPLIBDIR)/$(HTTPLIB_H)

catch2:	cleancatch2 $(CATCH2DIR) $(CATCH2ALL)

clean:	cleanobj cleantest
	$(RM) $(exe_file)
	$(RM) $(exe_test)
	rmdir -p $(BUILDDIR)/

cleanall:	clean
	$(RM) $(bin_file)

cleanobj:	$(BUILDDIR)
	$(RM) $(bld_files)

cleanbackup:	$(ATTICDIR)
	$(RM) $(att_files)
	-rmdir $(ATTICDIRS) 2>/dev/null
	rmdir -p $(ATTICDIR)

cleantest:	$(BUILDDIR)
	$(RM) $(bld_tests)

cleantests:	cleantest
	$(RM) $(bin_test)

cleandeps:	cleanhttplib cleancatch2

cleanhttplib:	$(HTTPLIBDIR)
	$(RM) $(HTTPLIBALL)
	rmdir -p $(HTTPLIBDIR)

cleancatch2:	$(CATCH2DIR)
	$(RM) $(CATCH2ALL)
	rmdir $(CATCH2DIR)

$(BUILDDIR):
	mkdir -p $@/

$(HTTPLIBDIR):
	mkdir -p $@/

$(CATCH2DIR):	$(TESTDIR)
	mkdir -p $@/

$(TESTDIR):
	mkdir -p $@/

$(ATTICDIR):
	mkdir -p $@/

$(att_files):	$(ATTICDIR)/%:	%
	@mkdir -p $(dir $@)
	cp -p $< $@

$(HTTPLIBALL):	$(HTTPLIBDIR)/%:	$(HTTPLIBDIR)
	@mkdir -p $(dir $@)
	curl -fsSL $(HTTPLIBSRC)/$(notdir $@) -o $@

$(CATCH2ALL):	$(CATCH2DIR)/%:	$(CATCH2DIR)
	@mkdir -p $(TESTDIR)
	@mkdir -p $(dir $@)
	curl -fsSL $(CATCH2SRC)/$(notdir $@) -o $@

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

$(bin_test):	$(exe_test)
	#mv -Tfuv $< $@
	cp -p $< $@

$(exe_test):	$(bld_tests)
	$(CXX) $^ $(LXXFLAGS) -o $@

$(bld_tests):	$(BUILDDIR)/%.o:	$(TESTDIR)/%.cpp $(TESTDIR)/%.hpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(TESTDIR) -c $< -o $@

