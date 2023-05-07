
SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.o, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_CCONTAINER_TESTS)

$(info == CCONTAINER UNIT_TEST : $(MODDIR_CCONTAINER_TESTS)==)
$(info $$SRCS_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))

unit_test :: test_clist_generic test_clist_cstring

# static library linkage is done as any other object file
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o $(MODDIR_CCONTAINER)/value.o libclogger
	@echo "Building test_clist_generic @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o $(MODDIR_CCONTAINER)/value.o -L $(LIB_CMOCKA) -lcmocka -L. -l$(IMPORT_LIB_CLOGGER)

# here test with public API only(don't include impl *c), must provide *.o dependencies
# or use libccontainer
OBJ_T_CLIST_CSTRING = $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o $(MODDIR_CCONTAINER)/clist_generic.o \
					$(MODDIR_CCONTAINER)/clist_cstring.o
test_clist_cstring: $(OBJ_T_CLIST_CSTRING) libclogger
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(OBJ_T_CLIST_CSTRING) -L. -l$(IMPORT_LIB_CLOGGER) -L $(LIB_CMOCKA) -lcmocka

# use only API, add all objects to simplify for a small library
test_cvector_generic_with_string : $(MODDIR_CCONTAINER_TESTS)/test_cvector_generic_with_string.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_generic_with_string @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# to use library later
test_cvector_struct_complex : $(MODDIR_CCONTAINER_TESTS)/test_cvector_struct_complex.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_struct_complex @ :    $@"
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "clean unit_test ccontainer"
	rm -f $(OBJS_CCONTAINER_TESTS)
	rm -f test_clist_generic test_clist_cstring test_cvector_generic_with_string test_cvector_struct_complex