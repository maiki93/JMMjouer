


# or defined by module record, like ccontainer
#MODDIR_TESTS = record/test
#MODDIR_RECORD_TESTS 

MODDIR_RECORD_TESTS := $(MODDIR_RECORD)/tests

$(info)
$(info == RECORD UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))
$(info $$MODDIR_RECORD_TESTS is $(MODDIR_RECORD_TESTS))
#$(info dir: $(dir $(lastword $(MAKEFILE_LIST))) )

# seems very convenient !
#currentDir = $(dir $(lastword $(MAKEFILE_LIST)))

# If UT are specialized, make sense again to use generic way in some case
SRCS_TESTS := $(wildcard $(MODDIR_RECORD_TESTS)/*.c)
$(info $$SRCS_TESTS is [ $(SRCS_TESTS) ])
OBJS_TESTS := $(patsubst %.c, %.o, $(SRCS_TESTS)) 
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])

# Override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in record tests :   $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I. -I $(INCLUDE_CMOCKA)

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
#unit_test:: $(EXE_TESTS_BIN)

unit_test :: test_file_record

# shared library available
test_file_record: $(MODDIR_RECORD_TESTS)/test_file_record.o libjoueur.dll librecord.dll 
	@echo "Building test_file_record @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_RECORD_TESTS)/test_file_record.o -L. -lrecord -ljoueur -L $(LIB_CMOCKA) -lcmocka

clean ::
#	rm -f $(OBJ_TESTS)
	rm -f $(MODDIR_RECORD_TESTS)/*.o