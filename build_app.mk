SOURCES      := $(wildcard $(CURDIR)/sources/*.cc)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cc)
SOURCES      += $(wildcard $(CURDIR)/sources/audio/*.cc)
SOURCES      += $(wildcard $(CURDIR)/sources/audio/codecs/*.cc)
SOURCES      += $(wildcard $(CURDIR)/sources/audio/debug/*.cc)

OBJECTS      := $(notdir $(patsubst %.cc,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources
VPATH        := $(VPATH):$(CURDIR)/sources/utils
VPATH        := $(VPATH):$(CURDIR)/sources/audio
VPATH        := $(VPATH):$(CURDIR)/sources/audio/codecs
VPATH        := $(VPATH):$(CURDIR)/sources/audio/debug

INCLUDE_PATH += -I$(CURDIR)/sources
INCLUDE_PATH += -I$(CURDIR)/sources/includes

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