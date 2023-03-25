
MODDIR_RECORD_TESTS := $(MODDIR_RECORD)/tests

SRCS_RECORD_TESTS := $(wildcard $(MODDIR_RECORD_TESTS)/*.c)
OBJS_RECORD_TESTS := $(patsubst %.c, %.o, $(SRCS_RECORD_TESTS)) 

OBJS_ALL_TESTS += $(OBJS_RECORD_TESTS)

$(info == RECORD UNIT_TEST : $(MODDIR_RECORD_TESTS) ==)
$(info $$SRCS_TESTS is [ $(SRCS_RECORD_TESTS) ])
$(info $$OBJS_TESTS is [$(OBJS_RECORD_TESTS)])
#$(info dir: $(dir $(lastword $(MAKEFILE_LIST))) )


# check existence of target(root)/test_X, avoid to rebuild all tests each time called
#unit_test:: $(EXE_TESTS_BIN)
unit_test :: test_file_record

# shared library available
test_file_record: $(MODDIR_RECORD_TESTS)/test_file_record.o libjoueur librecord libccontainer libclogger
	@echo "Building test_file_record @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_RECORD_TESTS)/test_file_record.o \
		-L. -lrecord -ljoueur -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean unit_test record"
	rm -f $(OBJS_RECORD_TESTS)
	rm -f test_file_record


