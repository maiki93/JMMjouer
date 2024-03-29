
MODDIR_RECORD_TESTS := $(MODDIR_RECORD)/tests

SRCS_RECORD_TESTS := $(wildcard $(MODDIR_RECORD_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_RECORD_TESTS := $(patsubst %.c, %.o, $(SRCS_RECORD_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_RECORD_TESTS)

$(info == RECORD UNIT_TEST : $(MODDIR_RECORD_TESTS) ==)
$(info $$SRCS_RECORD_TESTS is [ $(SRCS_RECORD_TESTS) ])
$(info $$OBJS_RECORD_TESTS is [$(OBJS_RECORD_TESTS)])

unit_test :: test_file_record

test_file_record : $(MODDIR_RECORD_TESTS)/test_file_record.o $(MODDIR_RECORD)/irecord.o  libclogger libccontainer libjoueur
	@echo "Building test_file_record @ :    $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_RECORD_TESTS)/test_file_record.o $(MODDIR_RECORD)/irecord.o -L . -ljoueur -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean module record tests"
	rm -f $(OBJS_RECORD_TESTS)
	rm -f test_file_record