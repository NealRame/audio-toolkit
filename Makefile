TARGET        = audio-metada
CC            = clang		
CXX           = clang++
COMMON_FLAGS  = -Wall -Werror -I$(CURDIR)/vendors/utf8
CFLAGS        = $(COMMON_FLAGS)
CXXFLAGS      = -std=c++11 $(COMMON_FLAGS)
LFLAGS        = -lmpg123
DEPS         := $(CURDIR)/Makefile.depends

export

.PHONY: all clean realclean Debug Release Tests

all: Debug Release

Debug Release:
	@$(MAKE) --no-print-directory -f build_app.mk $@

Tests:
	@$(MAKE) --no-print-directory -f build_tests.mk

realclean: clean
	rm -fr $(DEPS)
	rm -fr log
	rm -fr tags
	rm -fr Debug
	rm -fr Release
	rm -fr Tests
	
clean:
	rm -fr *~
	rm -fr 
	rm -fr test_detail.xml