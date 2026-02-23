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
USRINCDIR  := /usr/include
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
SETTINGS   := $(VSCODEDIR)/settings.json

# server source files
server_exe := server
server_main := main.cpp
mod_name   := pgh
mod_version := 1
mod_suff   := .$(mod_version)
mod_test   := test
test_ver   := 1
test_suff  := .$(test_ver)
server_cpp := \
	      nicesvc.cpp \
	      httpsrv.cpp \
	      dbconn.cpp \
	      dbparam.cpp \
	      dbtupel.cpp \
	      dbrecset.cpp \
	      dbanswer.cpp \
	      dbstmt.cpp \
		  dbquery.cpp \
	      pgconn.cpp \
	      pgparam.cpp \
	      pgtupel.cpp \
	      pgrecset.cpp \
	      pganswer.cpp \
	      pgstmt.cpp \
		  pgquery.cpp \
# EOL server source files

# test source files
test_exe   := tests
test_main  := test_dummy.cpp
test_cpp   := \
	      test_poc.cpp \
# EOL test source files

# server dependency on $(DBINC)
server_inc := nicesvc.cpp httpsrv.cpp dbconn.cpp dbparam.cpp dbtupel.cpp dbrecset.cpp dbanswer.cpp dbstmt.cpp dbquery.cpp pgconn.cpp pgparam.cpp pgtupel.cpp pgrecset.cpp pganswer.cpp pgstmt.cpp pgquery.cpp
db_inc     := dbanswer.cpp dbconn.cpp dbparam.cpp dbtupel.cpp dbrecset.cpp dbstmt.cpp dbquery.cpp
pg_inc     := pganswer.cpp pgconn.cpp pgparam.cpp pgtupel.cpp pgrecset.cpp pgstmt.cpp pgquery.cpp
httplib_inc := httpsrv.cpp
catch2_inc := test_dummy.cpp

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

lib_static := lib$(mod_name).a
lib_dyn    := lib$(mod_name).so$(mod_suff)

#lib_static := lib$(mod_name)$(BUILDSUFF).a
#lib_dyn    := lib$(mod_name)$(BUILDSUFF).so$(mod_suff)

test_static := lib$(mod_test).a
test_dyn   := lib$(mod_test).so$(mod_suff)

lib_file   := $(BUILDDIR)/$(lib_static)
so_file    := $(BUILDDIR)/$(lib_dyn)

lib_test   := $(BUILDDIR)/$(test_static)
so_test    := $(BUILDDIR)/$(test_dyn)

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
CPPFLAGS   := -fPIC
#CPPFLAGS   := 
#CPPFLAGS   := -I/usr/include/postgresql
CPPINC     := -I $(INCLUDEDIR)
TESTINC    := -I $(TESTDIR)
CPPOPTS    := $(CPPSTYLE) $(CFLAGS) $(CPPINC) $(CPPFLAGS)
#CPPOPTS    := $(CPPSTYLE)
LXXFLAGS   := $(CPPOPTS) $(CXXCONFIG) $(CXXLIBS)
CXXFLAGS   := $(CPPOPTS) $(CXXCONFIG) 

PGLIBPQ    := postgresql/libpq-fe.h
PGCONFIG   := postgresql/pg_config.h
PGLIBDAT   := $(PGLIBPQ) $(PGCONFIG)
PGLIBINC   := $(PGLIBDAT:%=$(USRINCDIR)/%)
PGLIBALL   := $(PGLIBINC)

DBDAT      := db_type.h
DBINC      := $(DBDAT:%=$(INCLUDEDIR)/%)
DBALL      := $(DBINC)

PGDAT      := pg_type.h
PGINC      := $(PGDAT:%=$(INCLUDEDIR)/%) $(PGLIBINC)
PGALL      := $(PGINC)

HTTPLIBDOC := README.md
HTTPLIBLIC := LICENSE
HTTPLIB_H  := httplib.h
HTTPLIBSRC := https://raw.githubusercontent.com/yhirose/cpp-httplib/refs/heads/master
HTTPLIBDAT := $(HTTPLIB_H) $(HTTPLIBDOC) $(HTTPLIBLIC)
HTTPLIBINC := $(HTTPLIB_H:%=$(INCLUDEDIR)/%)
HTTPLIBALL := $(HTTPLIBDAT:%=$(HTTPLIBDIR)/%)

CATCH2_H   := catch.hpp
CATCH2SRC  := https://raw.githubusercontent.com/catchorg/Catch2/v2.13.10/single_include/catch2
CATCH2DAT  := $(CATCH2_H)
CATCH2INC  := $(CATCH2_H:%=$(CATCH2DIR)/%)
CATCH2ALL  := $(CATCH2INC)

cpp_files  := $(server_cpp)
obj_files  := $(cpp_files:.cpp=.o)
hpp_files  := $(cpp_files:.cpp=.hpp)
inc_files  := $(obj_files:%.o=$(INCLUDEDIR)/%.hpp)
src_files  := $(obj_files:%.o=$(SRCDIR)/%.cpp)
main_file  := $(server_main:%.cpp=$(SRCDIR)/%.cpp)
head_file  := $(server_main:%.cpp=$(INCLUDEDIR)/%.hpp)
bld_files  := $(obj_files:%.o=$(BUILDDIR)/%.o)
all_files  := $(bin_file)
cpp_tests  := $(test_cpp)
obj_tests  := $(cpp_tests:.cpp=.o)
hpp_tests  := $(cpp_tests:.cpp=.hpp)
inc_tests  := $(obj_tests:%.o=$(TESTDIR)/%.hpp)
src_tests  := $(obj_tests:%.o=$(TESTDIR)/%.cpp)
main_test  := $(test_main:%.cpp=$(TESTDIR)/%.cpp)
head_test  := $(test_main:%.cpp=$(TESTDIR)/%.hpp)
bld_tests  := $(obj_tests:%.o=$(BUILDDIR)/%.o)
all_tests  := $(bin_test)
sav_files  := $(MAKEFILE) $(README) $(GITIGNORE) $(LAUNCH) $(TASKS) $(SETTINGS) $(HTTPLIBINC) $(CATCH2INC) $(main_file) $(head_file) $(main_test) $(head_test) $(src_files) $(inc_files) $(src_tests) $(inc_tests)
att_files  := $(sav_files:%=$(ATTICDIR)/%)

db_dep     := $(db_inc:%.cpp=$(BUILDDIR)/%.o)
pg_dep     := $(pg_inc:%.cpp=$(BUILDDIR)/%.o)
httplib_dep := $(httplib_inc:%.cpp=$(BUILDDIR)/%.o)
catch2_dep := $(catch2_inc:%.cpp=$(BUILDDIR)/%.o)

DEFAULT    := $(exe_file)

.DEFAULT_GOAL := default

.MAIN:	default

.PHONY:	default all build buildall clean cleanall cleanobj cleanbackup cleantest cleantests cleandeps cleanhttplib cleancatch2 makedebug makerelease makebin installdebug installrelease installbin builddebug buildrelease buildbin configs configsmake configsbuild configsall install installdebug run rundebug testsrun testsrundebug tests testsdebug lib so bylib testslib testsso testsbylib testsbylib deps httplib catch2 backup

default:	$(BUILDDIR) $(DEFAULT)

all:	deps install backup

build:	cleanall install tests

buildall:	deps cleanall install tests backup

makedebug:
	$(MAKE) DEBUG=1 default lib so tests

makerelease:
	$(MAKE) DEBUG=0 default lib so tests

makebin:
	$(MAKE) DEBUG=2 default lib so tests

installdebug:
	$(MAKE) DEBUG=1 install

installrelease:
	$(MAKE) DEBUG=0 install

installbin:
	$(MAKE) DEBUG=2 install

builddebug:
	$(MAKE) DEBUG=1 build lib so tests

buildrelease:
	$(MAKE) DEBUG=0 build lib so tests

buildbin:
	$(MAKE) DEBUG=2 build lib so tests

configs:	installdebug installrelease installbin

configsmake:	makedebug makerelease makebin

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

testsdebug:
	$(MAKE) DEBUG=1 tests

testsrun:	tests
	$(bin_test)

testsrundebug:	testsdebug
	gdb $(bin_test)

lib:	$(lib_file)

so:	$(so_file)

testslib:	$(lib_test)

testsso:	$(so_test)

deps:	httplib $(CATCH2INC)

httplib:	cleanhttplib $(HTTPLIBDIR) $(HTTPLIBALL)
	-diff -b $(HTTPLIBINC) $(HTTPLIBDIR)/$(HTTPLIB_H)

catch2:	cleancatch2 $(CATCH2DIR) $(CATCH2ALL)

clean:	cleanobj cleantest
	$(RM) $(exe_file)
	$(RM) $(exe_test)
	$(RM) $(lib_file)
	$(RM) $(so_file)
	$(RM) $(lib_test)
	$(RM) $(so_test)
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

$(exe_file):	$(main_file) $(bld_files)
	$(CXX) $^ $(LXXFLAGS) -o $@

bylib:	$(main_file) $(lib_file)
	$(CXX) $< -L$(BUILDDIR) -l$(mod_name) $(LXXFLAGS) -o $(exe_file)

$(main_file):	$(head_file)

$(lib_file):	$(bld_files)
	$(AR) rcs $@ $^

$(so_file):	$(bld_files)
	$(CXX) -shared $^ $(LXXFLAGS) -o $@

#$(lib_file):	$(bld_files)
#	$(CXX) -shared -Wl,-soname,$(lib_dyn) $^ $(LXXFLAGS) -o $@

#$(so_file):	$(bld_files)
#	$(CXX) -shared -Wl,-soname,$(lib_dyn) $^ $(LXXFLAGS) -o $@

$(bld_files):	$(BUILDDIR)/%.o:	$(SRCDIR)/%.cpp $(INCLUDEDIR)/%.hpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(db_dep):	$(BUILDDIR)/%.o:	$(DBINC)

$(pg_dep):	$(BUILDDIR)/%.o:	$(PGINC)

$(httplib_dep):	$(BUILDDIR)/%.o:	$(HTTPLIBINC)

$(catch2_dep):	$(BUILDDIR)/%.o:	$(CATCH2INC)

$(bin_test):	$(exe_test)
	#mv -Tfuv $< $@
	cp -p $< $@

$(exe_test):	$(main_test) $(bld_tests) $(bld_files)
	$(CXX) $^ $(LXXFLAGS) $(TESTINC) -o $@

testsbylib:	$(main_test) $(lib_test) $(lib_file)
	$(CXX) $< -L$(BUILDDIR) -l$(mod_test) -l$(mod_name) $(LXXFLAGS) $(TESTINC) -o $(exe_test)

$(main_file):	$(head_file)

$(lib_test):	$(bld_tests)
	$(AR) rcs $@ $^

$(so_test):	$(bld_tests)
	$(CXX) -shared $^ $(LXXFLAGS) -o $@

$(bld_tests):	$(BUILDDIR)/%.o:	$(TESTDIR)/%.cpp $(TESTDIR)/%.hpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(TESTINC) -c $< -o $@

