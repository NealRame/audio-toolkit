SOURCES      := $(wildcard $(CURDIR)/sources/audio/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/tests/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cpp)

OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources/audio
VPATH        := $(VPATH):$(CURDIR)/sources/tests
VPATH        := $(VPATH):$(CURDIR)/sources/utils

INCLUDE_PATH  = -I$(CURDIR)/sources
INCLUDE_PATH += -isystem $(CURDIR)/vendors/gtest/include 
INCLUDE_PATH += -I$(CURDIR)/vendors/gtest
INCLUDE_PATH += -isystem $(CURDIR)/vendors/gmock/include
INCLUDE_PATH += -I$(CURDIR)/vendors/gmock

CFLAGS       += $(INCLUDE_PATH)
CXXFLAGS     += $(INCLUDE_PATH)

export

.PHONY: all

all: Makefile.depends
	@mkdir -p Tests
	$(MAKE) --no-print-directory -C Tests -f ../Tests.mk

Makefile.depends: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)
