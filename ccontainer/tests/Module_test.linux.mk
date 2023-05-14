
SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.o, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_CCONTAINER_TESTS)

$(info == CCONTAINER UNIT_TEST : $(MODDIR_CCONTAINER_TESTS)==)
$(info $$SRCS_CCONTAINER_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
$(info $$OBJS_CCONTAINER_TESTS is [$(OBJS_CCONTAINER_TESTS)])

# from SRCS possible
unit_test :: test_clist_generic test_clist_cstring

# adapt to static or shared  library
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o libclogger
	@echo "Building test_clist_generic @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o -L . -lclogger -L $(LIB_CMOCKA) -lcmocka

# here test with public API only(don't include impl *c), must provide *.o dependencies
test_clist_cstring: $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o libccontainer libclogger
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o -L . -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

test_cvector_generic_with_string : $(MODDIR_CCONTAINER_TESTS)/test_cvector_generic_with_string.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_generic_with_string @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

test_cvector_struct_complex : $(MODDIR_CCONTAINER_TESTS)/test_cvector_struct_complex.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_struct_complex @ :    $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "clean unit_test ccontainer"
	rm -f $(OBJS_CCONTAINER_TESTS)
	rm -f test_clist_generic test_clist_cstring test_cvector_generic_with_string test_cvector_struct_complex
