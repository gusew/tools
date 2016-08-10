include make_vars.mk

.PHONY: all
all: $(BUILD)writeread $(BUILD)readwrite $(BUILD)readcalc $(BUILD)seekjump $(BUILD)writewrite

# build all tools in file i/o suite
$(BUILD)writeread: $(BUILD)writeread.o
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)readwrite: $(BUILD)readwrite.o
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)readcalc: $(BUILD)readcalc.o
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)seekjump: $(BUILD)seekjump.o
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)writewrite: $(BUILD)writewrite.o
	$(CC) $(CFLAGS) $< -o $@

# |
# | *** Compile files ***
# | 
$(BUILD)%.o: $(SRC)%.cpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: valtest_wr valtest_rw valtest_rc valtest_sj valtest_ww clean

valtest_wr: $(BUILD)writeread
	valgrind $(VALFLAGS) $(BUILD)writeread /tmp/test_fio_writeread 10000
	rm /tmp/test_fio_writeread

valtest_rw: $(BUILD)readwrite
	echo "Hello World and Valgrind!" > /tmp/test_fio_readwrite 
	valgrind $(VALFLAGS) $(BUILD)readwrite /tmp/test_fio_readwrite
	rm /tmp/test_fio_readwrite

valtest_rc: $(BUILD)readcalc
	echo "Hello World and Valgrind!" > /tmp/test_fio_readcalc 
	valgrind $(VALFLAGS) $(BUILD)readcalc /tmp/test_fio_readcalc
	rm /tmp/test_fio_readcalc

valtest_sj: $(BUILD)seekjump
	echo "You can jump many times through this text, just for fun and testing purposes. Enjoy jumping!" > /tmp/test_fio_seekjump
	valgrind $(VALFLAGS) $(BUILD)seekjump /tmp/test_fio_seekjump
	rm /tmp/test_fio_seekjump

valtest_ww: $(BUILD)writewrite
	valgrind $(VALFLAGS) $(BUILD)writewrite /tmp/test_fio_writewrite1 /tmp/test_fio_writewrite2 10000
	rm /tmp/test_fio_writewrite1 /tmp/test_fio_writewrite2

clean:
	-@rm -r $(BUILD)

