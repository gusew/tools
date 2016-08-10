include make_vars.mk

.PHONY: run
run: $(BUILD)$(APPNAME)
	$(BUILD)$(APPNAME) 100000

$(BUILD)$(APPNAME): $(BUILD)main.o
	$(CC) $(CFLAGS) $< -o $@

# |
# | *** Compile files ***
# | 
$(BUILD)%.o: $(SRC)%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: valtest clean

valtest: $(BUILD)$(APPNAME)
	valgrind $(VALFLAGS) $(BUILD)$(APPNAME) 10000

clean:
	-@rm -r $(BUILD)

