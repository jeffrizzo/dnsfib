SRCDIR:=src
OBJDIR:=obj
TESTDIR:=tests
Q?=@

# Append to flags in case we want to set from env.  Turn on
# all warnings and make them errors.
CFLAGS+= -Wall -Werror -Wextra
CPPFLAGS+=

# where does the cmocka test library live?
CMOCKA_BASE?=/usr/pkg
CMOCKA_INCLUDE?=${CMOCKA_BASE}/include
CMOCKA_LIB?=${CMOCKA_BASE}/lib

TEST_CFLAGS:= $(CFLAGS) -Wno-unused-parameter -Wno-pointer-to-int-cast
TEST_CPPFLAGS:= -DUNIT_TESTING -I$(CMOCKA_INCLUDE) $(CPPFLAGS)
TEST_LFLAGS:= -L$(CMOCKA_LIB) -lcmocka

# "dnsfib" because it's lying
BIN:= dnsfib

SRC:= $(wildcard $(SRCDIR)/*.c)
OBJ:= $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=.o))
TEST:= $(wildcard $(TESTDIR)/*.c)

$(BIN): $(OBJ)
	@echo LINK: $@
	$(Q)$(CC) -o $@ $(OBJ)

# Unit tests built with cmocka support.  We remove the test binaries after running
# the tests to keep things tidy.
.PHONY: test
test: test_main test_mysocket test_dnspkt test_handle func_test

.PHONY: test_main
test_main: $(TESTDIR)/test_main.c $(SRCDIR)/main.c
	$(Q)$(CC) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $< -o $(TESTDIR)/test_main $(TEST_LFLAGS)
	-$(TESTDIR)/test_main
	-@rm -f $(TESTDIR)/test_main

.PHONY: test_mysocket
test_mysocket: $(TESTDIR)/test_mysocket.c $(SRCDIR)/mysocket.c
	$(Q)$(CC) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $< -o $(TESTDIR)/test_mysocket $(TEST_LFLAGS)
	-$(TESTDIR)/test_mysocket
	-@rm -f $(TESTDIR)/test_mysocket

.PHONY: test_dnspkt
test_dnspkt: $(TESTDIR)/test_dnspkt.c $(SRCDIR)/dnspkt.h
	$(Q)$(CC) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $< -o $(TESTDIR)/test_dnspkt $(TEST_LFLAGS)
	-$(TESTDIR)/test_dnspkt
	-@rm -f $(TESTDIR)/test_dnspkt

.PHONY: test_handle
test_handle: $(TESTDIR)/test_handle.c $(SRCDIR)/handle.c
	$(Q)$(CC) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $< -o $(TESTDIR)/test_handle $(TEST_LFLAGS)
	-$(TESTDIR)/test_handle
	-@rm -f $(TESTDIR)/test_handle

.PHONY: func_test
func_test: $(TESTDIR)/testrun.sh
	@echo running test script
	/bin/sh $(TESTDIR)/testrun.sh ./$(BIN)

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)
	rmdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo COMPILE: $<
	$(Q)$(CC) $(DEPFLAGS) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

