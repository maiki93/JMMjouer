
$(info)
$(info == CCONTAINER UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))
$(info $$MODDIR_CCONTAINER_TESTS is $(MODDIR_CCONTAINER_TESTS))

SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.o, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])

OBJS_ALL_TESTS += $(OBJS_CCONTAINER_TESTS)

# Override generic rules for this directory, all format tests/*.c *.o 
# missing cmocka include otherwise
#$(OBJS_CCONTAINER_TESTS): %.o: %.c
#	@echo "Build *.o overriden generic rules in ccontainer tests:   $@"
#	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I . -I $(INCLUDE_CMOCKA)

# from SRCS possible
unit_test :: test_clist_generic test_clist_cstring

# static library linkage is done as any other object file
#test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o libclogger.a
#	@echo "Building test_clist_generic @ :    $@"  # target name
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# adapt to static or shared  library
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o libclogger
	@echo "Building test_clist_generic @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o -L . -lclogger -L $(LIB_CMOCKA) -lcmocka


# here test with public API only(don't include impl *c), must provide *.o dependencies
# very long line , try to include the library
#test_clist_cstring: $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o $(MODDIR_CCONTAINER)/clist_generic.o \
#					$(MODDIR_CCONTAINER)/clist_cstring.o libclogger
#	@echo "Building test_clist_cstring @ :    $@"  # target name
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L . -lclogger -L $(LIB_CMOCKA) -lcmocka

test_clist_cstring: $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o libccontainer libclogger
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o -L . -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka


clean ::
	rm -f $(OBJS_CCONTAINER_TESTS)
	rm -f test_clist_generic test_clist_cstring
