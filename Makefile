SRCDIR:=src
OBJDIR:=obj
Q?=@

BIN:= prog

SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ := $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=.o))

$(BIN): $(OBJ)
	@echo LINK: $@
	$(Q)$(CC) -o $@ $(OBJ)
	
clean:
	rm -f $(OBJ) $(BIN)
	rmdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo COMPILE: $<
	$(Q)$(CC) $(DEPFLAGS) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

