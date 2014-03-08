SOURCES      := $(wildcard $(CURDIR)/sources/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/audio/*.cpp)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cpp)

OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources
VPATH        := $(VPATH):$(CURDIR)/sources/audio
VPATH        := $(VPATH):$(CURDIR)/sources/utils

INCLUDE_PATH += -I$(CURDIR)/sources

CFLAGS       += $(INCLUDE_PATH)
CXXFLAGS     += $(INCLUDE_PATH)

export

.PHONY: all Debug Release

all: Debug Release

Debug Release: Makefile.depends
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $@ -f ../$@.mk $(TARGET)

Makefile.depends: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)
