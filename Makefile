SRCDIR:=src
OBJDIR:=obj
TESTDIR:=tests
#Q?=@

# where does the cmocka test library live?
CMOCKA_BASE?=/opt/brew
TEST_CFLAGS:= -I$(CMOCKA_BASE)/include
TEST_CPPFLAGS:= -DUNIT_TESTING
TEST_LFLAGS:= -L$(CMOCKA_BASE)/lib -lcmocka

BIN:= prog

SRC:= $(wildcard $(SRCDIR)/*.c)
OBJ:= $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=.o))
TEST:= $(wildcard $(TESTDIR)/*.c)

$(BIN): $(OBJ)
	@echo LINK: $@
	$(Q)$(CC) -o $@ $(OBJ)
	
# Unit tests built with cmocka support.  We remove the test binaries after running
# the tests to keep things tidy.
.PHONY: test
test: test_main

.PHONY: test_main
test_main: $(TESTDIR)/test_main.c $(SRCDIR)/main.c
	$(Q)$(CC) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $< -o $(TESTDIR)/test_main $(TEST_LFLAGS)
	-$(TESTDIR)/test_main
	-@rm -f $(TESTDIR)/test_main

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)
	rmdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo COMPILE: $<
	$(Q)$(CC) $(DEPFLAGS) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

