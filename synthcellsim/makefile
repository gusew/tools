include make_vars.mk

.PHONY: all
all: \
	$(BUILD)$(TOOL_GEN) \
	$(BUILD)$(TOOL_STAT) \
	$(BUILD)$(TOOL_SIM) \
	$(BUILD)$(TOOL_RND) \
	$(BUILD)$(TOOL_VIS)
#	$(BUILD)$(TOOL_SPLIT) \
#	$(BUILD)$(TOOL_EVAL) \

# build all tools
$(BUILD)$(TOOL_GEN): $(BUILD)$(TOOL_GEN).o $(BUILD)FileHandler.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_RND): $(BUILD)$(TOOL_RND).o $(BUILD)FileHandler.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_VIS): $(BUILD)$(TOOL_VIS).o $(BUILD)FileHandler.o $(BUILD)Grid.o $(BUILD)GridSerializer.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_SPLIT): $(BUILD)$(TOOL_SPLIT).o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_EVAL): $(BUILD)$(TOOL_EVAL).o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_SIM): $(BUILD)$(TOOL_SIM).o  $(BUILD)Grid.o $(BUILD)FileHandler.o $(BUILD)GridSerializer.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(TOOL_STAT): $(BUILD)$(TOOL_STAT).o $(BUILD)Grid.o $(BUILD)FileHandler.o $(BUILD)GridSerializer.o
	$(CC) $(CFLAGS) $^ -o $@

# | *** Compile files ***
$(BUILD)%.o: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create and run all unit tests
.PHONY: utest
utest: $(TESTS_ALL)
	@echo "\nStart unit tests for the Grid class."
	@$(TESTDIR)Grid.test

# link object files for unit tests
$(TESTDIR)Grid.test: $(BUILD)Grid.to 
	$(MKDIR)
	$(CC) $^ -o $@

# create object files for unit tests
$(BUILD)%.to: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) $(TESTFLAG) -c $< -o $@


#.PHONY: valtest_wr valtest_rw valtest_rc valtest_sj valtest_ww clean
.PHONY: clean

#valtest_wr: $(BUILD)writeread
#	valgrind $(VALFLAGS) $(BUILD)writeread /tmp/test_fio_writeread 10000
#	rm /tmp/test_fio_writeread

clean:
	-@rm -r $(BUILD)

