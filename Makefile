LIBRARIES =
INCLUDE = -I. -Iinclude/
CFLAGS = -Wall -Werror $(INCLUDE) $(LIBRARIES) -g3 \
         -MMD -MF $(DEPDIR)/$*.d

DEPDIR = .deps/

SOURCE ?= $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCE))

TEST_SOURCE = $(wildcard tests/*.c)
TEST_OBJECTS = $(patsubst %.c,%.o,$(TEST_SOURCE))
TESTS = $(patsubst %.c,%,$(TEST_SOURCE))

DEPENDS += $(patsubst %.c,$(DEPDIR)/%.d,$(SOURCE)) $(patsubst %.c,$(DEPDIR)/%.d,$(TEST_SOURCE))

LIBRARY = libds.a

all: $(LIBRARY) $(TESTS)

%.d:
	@mkdir -p $(@D)

%.o: %.c Makefile
	gcc $(CFLAGS) -c -o $@ $<

$(LIBRARY): $(OBJECTS)
	ar -rc $@ $^

test_%: test_%.o $(LIBRARY)
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(DEPDIR) $(OBJECTS) $(BINARY)

include $(DEPENDS)
