include makefile.vars

# | *** Link all compiled object files to executable ***
$(BUILD)$(APPNAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ 

# | *** Compile files ***
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

# | *** Targets for js compilation ***
.PHONY: jscompile
jscompile: $(BUILD)$(APPNAME).js

$(BUILD)$(APPNAME).js: $(OBJECTS_JS)
	$(EMCC) $(EMCCFLAGS) $^ -o $@

$(BUILD)js/%.o: $(SRC)%.cpp $(SRC)%.hpp
	$(MKDIR)
	$(EMCC) $(EMCCFLAGS) -c $< -o $@

