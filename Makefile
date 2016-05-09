TOP:=$(shell pwd)
LIBNAME=parser

#
CPPLIB_BASE=/home/dd/develop/c++/
LIBESTD_CXXFLAGS=-I$(CPPLIB_BASE)
# LIBESTD_LDFLAGS=-L$(CPPLIB_BASE)/estd/.libs/ -lestd
# LIBESTD_LD_LIBRARY_PATH=$(CPPLIB_BASE)/estd/.libs/


AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CXX		= $(CROSS_COMPILE)g++
# CXX		= $(CROSS_COMPILE)clang++
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
CFLAGS+=$(DEPFLAGS)
CXXFLAGS+=$(DEPFLAGS)
CXXFLAGS+=-I$(TOP)/..
CXXFLAGS+=-I$(TOP)/../..
# CXXFLAGS+=$(LIBESTD_CXXFLAGS)
CXXFLAGS+=-std=c++0x
CXXFLAGS+=-g
# CXXFLAGS+=-Wold-style-cast
# CXXFLAGS+=-O3
CXXFLAGS+=-fno-inline
LDFLAGS+=-lncurses
LDFLAGS+=-lpcrecpp -lpcre
# LDFLAGS+=$(LIBESTD_LDFLAGS)
LIB_EXPORT_DIR:=.libs


FLTKCONFIG   = fltk-config
FLTKCXX          = $(shell $(FLTKCONFIG) --cxx)
FLTKCXXFLAGS     = $(shell $(FLTKCONFIG) --cxxflags) -Wall -I.
LINKFLTK     = $(shell $(FLTKCONFIG) --ldstaticflags)
LINKFLTK_GL  = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)
LINKFLTK_IMG = $(shell $(FLTKCONFIG) --use-images --ldstaticflags)
LINKFLTK_ALL = $(shell $(FLTKCONFIG) --use-images --use-gl --ldstaticflags)


TEST_CFLAGS+=$(DEPFLAGS)
TEST_CFLAGS+=-I$(TOP)
TEST_CXXFLAGS+=$(CXXFLAGS)
TEST_CXXFLAGS+=$(DEPFLAGS)
TEST_CXXFLAGS+=-I$(TOP)
TEST_CXXFLAGS+=-std=c++0x
TEST_CXXFLAGS+=-I/home/dd/project/gtest-1.7.0/include/
TEST_LDFLAGS+=$(LDFLAGS)
TEST_LDFLAGS+=-lSegFault -lrt
TEST_LDFLAGS+=-L/home/dd/project/gtest-1.7.0/lib/.libs/ -lgtest -pthread
TEST_LDFLAGS+=-lglog
TEST_LDFLAGS+=-lpcrecpp -lpcre
TEST_LDFLAGS+=-L$(TOP)/$(LIB_EXPORT_DIR) -l$(LIBNAME)
TEST_LD_LIBRARY_PATH=/home/dd/project/gtest-1.7.0/lib/.libs/:$(TOP)/$(LIB_EXPORT_DIR)

DEMO_CFLAGS+=$(DEPFLAGS)
DEMO_CFLAGS+=-I$(TOP)
DEMO_CXXFLAGS+=$(DEPFLAGS)
DEMO_CXXFLAGS+=-I$(TOP)
DEMO_CXXFLAGS+=-std=c++0x
DEMO_CXXFLAGS+=$(CXXFLAGS)
# DEMO_CXXFLAGS+=$(FLTKCXXFLAGS)
DEMO_LDFLAGS+=$(LDFLAGS)
DEMO_LDFLAGS+=-L/home/dd/project/google/glog-0.3.3/.libs/ -lglog
DEMO_LDFLAGS+=-lSegFault -lrt
DEMO_LDFLAGS+=-L$(TOP)/$(LIB_EXPORT_DIR) -l$(LIBNAME)
# DEMO_LDFLAGS+=$(LINKFLTK)

MEMO_CFLAGS+=$(DEPFLAGS)
MEMO_CFLAGS+=-I$(TOP)
MEMO_CXXFLAGS+=$(DEPFLAGS)
MEMO_CXXFLAGS+=-I$(TOP)
MEMO_CXXFLAGS+=-std=c++0x
MEMO_LDFLAGS+=$(LDFLAGS)
MEMO_LDFLAGS+=$(LIB_EXPORT_DIR)/lib$(LIBNAME).a
MEMO_LDFLAGS+=-L$(TOP)/$(LIB_EXPORT_DIR) -l$(LIBNAME)

SCHEDULER_CFLAGS+=$(DEPFLAGS)
SCHEDULER_CFLAGS+=-I$(TOP)
SCHEDULER_CXXFLAGS+=$(CXXFLAGS)
SCHEDULER_CXXFLAGS+=$(DEPFLAGS)
SCHEDULER_CXXFLAGS+=-I$(TOP)
SCHEDULER_CXXFLAGS+=-std=c++0x
SCHEDULER_CXXFLAGS+=$(FLTKCXXFLAGS)
SCHEDULER_LDFLAGS+=$(LDFLAGS)
SCHEDULER_LDFLAGS+=$(LIB_EXPORT_DIR)/lib$(LIBNAME).a
SCHEDULER_LDFLAGS+=$(LINKFLTK)
SCHEDULER_LDFLAGS+=-lSegFault -lrt
# SCHEDULER_LDFLAGS+=-L$(TOP)/$(LIB_EXPORT_DIR) -l$(LIBNAME)

DEPDIR:=.deps
DEPFLAGS=-MMD -MP
#DEPFLAGS = -MMD -MF ${DEPDIR}/$*.d -MP
MKDEPDIR = ${MKDIR_P} ${DEPDIR}
MKDIR_P = /bin/mkdir -p

# subdirs
subdirs:=$(shell cd $(TOP); ls -d */)
subdirs:=$(patsubst %/,%,$(subdirs))
libs-subdirs:=$(filter-out demo unit_test plan,$(subdirs))
tests-subdirs:=$(filter unit_test,$(subdirs))
demo-subdirs:=$(filter demo,$(subdirs))
memo-subdirs:=$(filter memo,$(subdirs))
scheduler-subdirs:=$(filter scheduler,$(subdirs))

# srcs
libs-c-srcs:=$(foreach dir,$(libs-subdirs),$(wildcard $(dir)/*.c))
libs-cpp-srcs:=$(foreach dir,$(libs-subdirs),$(wildcard $(dir)/*.cpp))
tests-c-srcs:=$(foreach dir,$(tests-subdirs),$(wildcard $(dir)/*.c))
tests-cpp-srcs:=$(foreach dir,$(tests-subdirs),$(wildcard $(dir)/*.cpp))
demo-cpp-srcs:=$(foreach dir,$(demo-subdirs),$(wildcard $(dir)/*.cpp))
memo-cpp-srcs:=$(foreach dir,$(memo-subdirs),$(wildcard $(dir)/*.cpp))
scheduler-cpp-srcs:=$(foreach dir,$(scheduler-subdirs),$(wildcard $(dir)/*.cpp))

# objects
libs-c-objects:=$(patsubst %.c,%.o,$(libs-c-srcs))
libs-cpp-objects:=$(patsubst %.cpp,%.o,$(libs-cpp-srcs))
tests-c-objects:=$(patsubst %.c,%.o,$(tests-c-srcs))
tests-cpp-objects:=$(patsubst %.cpp,%.o,$(tests-cpp-srcs))
demo-cpp-objects:=$(patsubst %.cpp,%.o,$(demo-cpp-srcs))
memo-cpp-objects:=$(patsubst %.cpp,%.o,$(memo-cpp-srcs))
scheduler-cpp-objects:=$(patsubst %.cpp,%.o,$(scheduler-cpp-srcs))

libs-objects:=$(libs-c-objects) $(libs-cpp-objects)
tests-objects:=$(tests-c-objects) $(tests-cpp-objects)
demo-objects:=$(demo-cpp-objects)
memo-objects:=$(memo-cpp-objects)
scheduler-objects:=$(scheduler-cpp-objects)
objects:=$(libs-objects) $(tests-objects) $(demo-objects) $(memo-objects) $(scheduler-objects)

# dependence
libs-c-dependences:=$(patsubst %.c,%.d,$(libs-c-srcs))
libs-cpp-dependences:=$(patsubst %.cpp,%.d,$(libs-cpp-srcs))
tests-c-dependences:=$(patsubst %.c,%.d,$(tests-c-srcs))
tests-cpp-dependences:=$(patsubst %.cpp,%.d,$(tests-cpp-srcs))
demo-cpp-dependences:=$(patsubst %.cpp,%.d,$(demo-cpp-srcs))
memo-cpp-dependences:=$(patsubst %.cpp,%.d,$(memo-cpp-srcs))
scheduler-cpp-dependences:=$(patsubst %.cpp,%.d,$(scheduler-cpp-srcs))

libs-dependences:=$(libs-c-dependences) $(libs-cpp-dependences)
tests-dependences:=$(tests-c-dependences) $(tests-cpp-dependences)
demo-dependences:=$(demo-cpp-dependences)
memo-dependences:=$(memo-cpp-dependences)
scheduler-dependences:=$(scheduler-cpp-dependences)
dependences:=$(libs-dependences) $(tests-dependences) $(demo-dependences) $(memo-dependences) $(scheduler-dependences)

all: lib #unit_test # demo memo scheduler

$(libs-c-objects): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(libs-cpp-objects): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(tests-c-objects): %.o: %.c
	$(CC) -c $(TEST_CFLAGS) $< -o $@

$(tests-cpp-objects): %.o: %.cpp
	$(CXX) -c $(TEST_CXXFLAGS) $< -o $@

$(demo-cpp-objects): %.o: %.cpp
	$(CXX) -c $(DEMO_CXXFLAGS) $< -o $@

$(memo-cpp-objects): %.o: %.cpp
	$(CXX) -c $(MEMO_CXXFLAGS) $< -o $@

$(scheduler-cpp-objects): %.o: %.cpp
	$(CXX) -c $(SCHEDULER_CXXFLAGS) $< -o $@

lib:$(libs-objects)
	@mkdir -p $(LIB_EXPORT_DIR)
	$(AR) rcs $(LIB_EXPORT_DIR)/lib$(LIBNAME).a $(libs-objects)
	$(CXX) -shared $(libs-objects) -o $(LIB_EXPORT_DIR)/lib$(LIBNAME).so

unit_test:lib $(tests-objects)
	@echo $(subdirs-test)
	$(CXX) $(tests-objects) -o unit_test/test $(TEST_LDFLAGS)

demo:lib $(demo-objects)
	$(CXX) $(demo-objects) -o demo/demo $(DEMO_LDFLAGS)

memo:lib $(memo-objects)
	$(CXX) $(memo-objects) -o memo/memo $(MEMO_LDFLAGS)

scheduler:lib $(scheduler-objects)
	$(CXX) $(scheduler-objects) -o scheduler/scheduler $(SCHEDULER_LDFLAGS)

run_unit_test:unit_test
	LD_LIBRARY_PATH=$(TEST_LD_LIBRARY_PATH) ./unit_test/test

run_demo:demo
	LD_LIBRARY_PATH=$(TEST_LD_LIBRARY_PATH) ./demo/demo

run_memo:memo
	LD_LIBRARY_PATH=$(TOP)/$(LIB_EXPORT_DIR) ./memo/memo

run_scheduler:scheduler
	LD_LIBRARY_PATH=$(TOP)/$(LIB_EXPORT_DIR) ./scheduler/scheduler

clean:
	rm -rf $(objects) $(dependences) $(LIB_EXPORT_DIR)

.PHONEY: all

-include $(dependences)
