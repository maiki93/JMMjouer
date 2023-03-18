
SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.obj, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_CCONTAINER_TESTS)

$(info == CCONTAINER UNIT_TEST : $(CCONTAINER UNIT_TEST) ==)
$(info $$SRCS_CCONTAINER_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
$(info $$OBJS_CCONTAINER_TESTS is [$(OBJS_CCONTAINER_TESTS)])

# or patsubst from source
unit_test :: test_clist_generic test_clist_cstring

# behavior very different if use API or not ?
# include source file impl. many errors. Ok if no use of /DwithLIB (warning incoherent with lib)
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.obj $(IMPORT_LIB_CLOGGER)
	@echo "Building test_clist_generic @ :    $@"  # target name
	@echo "Building test_clist_generic ? :    $?"  # arguments
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

# include only header and link library, fine...
test_clist_cstring: $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.obj $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib


clean ::
#	del $(MODDIR_CCONTAINER_TESTS)\*.obj
	del ccontainer\tests\*.obj
	del test_clist_generic.exe, test_clist_cstring.exe
	del test_clist_generic.*, test_clist_cstring.*
