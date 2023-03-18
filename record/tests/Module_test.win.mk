
SRCS_RECORD_TESTS := $(wildcard $(MODDIR_RECORD_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_RECORD_TESTS := $(patsubst %.c, %.obj, $(SRCS_RECORD_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_RECORD_TESTS)

$(info == RECORD UNIT_TEST : $(MODDIR_RECORD_TESTS) ==)
$(info $$SRCS_RECORD_TESTS is [ $(SRCS_RECORD_TESTS) ])
$(info $$OBJS_RECORD_TESTS is [$(OBJS_RECORD_TESTS)])

unit_test :: test_file_record
 
#test_file_record: $(MODDIR_RECORD_TESTS)/test_file_record.obj $(IMPORT_LIB_RECORD) $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_JOUEUR)
# ok pass , need irecord.obj to get access private part not accessible from library
# need explicit clogger lib if libjoueur is not shared
test_file_record: $(MODDIR_RECORD_TESTS)/test_file_record.obj $(MODDIR_RECORD)/irecord.obj $(IMPORT_LIB_CCONTAINER) \
					$(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CLOGGER)
	@echo "Building test_file_record @ :    $@"  # target name
	@echo "Building test_file_record ^ :    $^"  
	@echo "Building test_file_record < :    $<"  
	@echo "Building test_file_record ? :    $?" 
	$(LINK) $(LFLAGS) $^ /LIBPATH:$(LIB_CMOCKA) cmocka.lib

clean ::
	del $(MODDIR_RECORD_TESTS)\*.obj
	del test_file_record.exe
