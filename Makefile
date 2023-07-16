LIBRARIES =
INCLUDE = -I. -Iinclude/ -Itests/ctest/ -Itests/ctest/Unity/src
CFLAGS = -Wall -Werror $(INCLUDE) $(LIBRARIES) -g3  -DUNITY_OUTPUT_COLOR \
         -MMD -MF $(DEPDIR)/$*.d

DEPDIR = .deps/

SOURCE ?= $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCE))

TEST_SOURCE = $(filter-out src/main.c, $(SOURCE)) \
              $(wildcard tests/*.c) \
              $(wildcard tests/ctest/*.c) \
              $(wildcard tests/ctest/Unity/src/*.c)

TEST_OBJECTS = $(patsubst %.c,%.o,$(TEST_SOURCE))

DEPENDS += $(patsubst %.c,$(DEPDIR)/%.d,$(SOURCE)) $(patsubst %.c,$(DEPDIR)/%.d,$(TEST_SOURCE))

LIBRARY = libds.a

all: $(LIBRARY) test

%.d:
	@mkdir -p $(@D)

%.o: %.c Makefile
	gcc $(CFLAGS) -c -o $@ $<

$(LIBRARY): $(OBJECTS)
	ar -rc $@ $^

test:  $(TEST_OBJECTS) $(LIBRARY)
	gcc $(CFLAGS) -T tests/ctest/test_cases_section.ld -o $@ $^

.PHONY: clean
clean:
	rm -rf $(DEPDIR) $(OBJECTS) $(BINARY) $(TEST_OBJECTS) $(TESTS)

include $(DEPENDS)
