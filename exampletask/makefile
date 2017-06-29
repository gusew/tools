include makefile.vars

# |
# | *** Run compiled and linked executable ***
# | 
.PHONY: run
run: $(BUILD)$(APPNAME) 
	$(BUILD)$(APPNAME) 

# |
# | *** Link all compiled object files to executable ***
# | 
$(BUILD)$(APPNAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBS)

# |
# | *** Compile files ***
# | 
$(BUILD)%.o: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@


# | *** Targets for other tools. ***
.PHONY: valtest 
valtest: $(BUILD)$(APPNAME)
	valgrind $(VALFLAGS) $(BUILD)$(APPNAME)

.PHONY: clean
clean:
	-@rm -r $(BUILD)

