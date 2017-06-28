include make_vars.mk

.PHONY: all
all: \
	$(BUILD)$(SERVER) \
	$(BUILD)$(CLIENT) \
	$(BUILD)$(ASYNC)

# build all tools
$(BUILD)$(SERVER): $(BUILD)$(SERVER).o $(BUILD)TcpSocket.o $(BUILD)TcpConnection.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(CLIENT): $(BUILD)$(CLIENT).o $(BUILD)TcpSocket.o $(BUILD)TcpConnection.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)$(ASYNC): $(BUILD)$(ASYNC).o
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
#$(TESTDIR)Grid.test: $(BUILD)Grid.to 
#	$(MKDIR)
#	$(CC) $^ -o $@

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

