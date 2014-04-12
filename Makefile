TARGET        = nrbuffer
CC            = g++

COMMON_FLAGS  = -Wall -Werror
CFLAGS        = $(COMMON_FLAGS)
CXXFLAGS      = -std=c++11 $(COMMON_FLAGS)

SOURCES      := $(wildcard $(CURDIR)/sources/*.cc)
SOURCES      += $(wildcard $(CURDIR)/sources/utils/*.cc)

OBJECTS      := $(notdir $(patsubst %.cc,%.o,$(SOURCES)))

VPATH        := $(CURDIR)/sources
VPATH        := $(VPATH):$(CURDIR)/sources/utils

DEPS         := $(CURDIR)/Makefile.depends

export

.PHONY: all clean Debug depends realclean Release

all: Debug Release

Debug Release:
	@mkdir -p $@
	$(MAKE) --no-print-directory -C $@ -f ../$@.mk $(TARGET)

depends: $(SOURCES)
	$(CC) $(CXXFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)

realclean: clean
	rm -fr log
	rm -fr tags
	rm -fr Debug
	rm -fr Release

clean:
	rm -fr *~
	rm -fr $(DEPS)
