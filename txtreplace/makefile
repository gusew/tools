include make_vars.mk

$(BUILD)$(PROGNAME): $(BUILD)main.o $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@


# | *** Compile files ***
$(BUILD)%.o: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create and run all unit tests
.PHONY: utest
utest: $(TESTS_ALL)
	@echo "\nStart unit tests for the search and replace function."
	@$(TESTDIR)SearchReplace.test

# link object files for unit tests
$(TESTDIR)SearchReplace.test: $(BUILD)SearchReplace.to $(BUILD)ReplaceInstructions.o
	$(MKDIR)
	$(CC) $^ -o $@

# create object files for unit tests
$(BUILD)%.to: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) $(TESTFLAG) -c $< -o $@


#.PHONY: valtest_wr valtest_rw valtest_rc valtest_sj valtest_ww clean

#valtest_wr: $(BUILD)writeread
#	valgrind $(VALFLAGS) $(BUILD)writeread /tmp/test_fio_writeread 10000
#	rm /tmp/test_fio_writeread

#-----------------------------------------------------------------------

.PHONY: deploy 
deploy: $(LINK_DIR)$(PROGNAME)
	@echo "The tool $(PROGNAME) was successfully deployed."

$(LINK_DIR)$(PROGNAME): $(DEPLOY_DIR)$(PROGNAME)
	ln -s $< $(LINK_DIR)$(PROGNAME)

$(DEPLOY_DIR)$(PROGNAME): $(BUILD)$(PROGNAME)
	mkdir $(DEPLOY_DIR)
	cp $< $(DEPLOY_DIR)

#-----------------------------------------------------------------------

.PHONY: clean
clean:
	-@rm -r $(BUILD)

