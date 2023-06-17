
SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.o, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_CCONTAINER_TESTS)

$(info == CCONTAINER UNIT_TEST : $(MODDIR_CCONTAINER_TESTS)==)
$(info $$SRCS_CCONTAINER_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
$(info $$OBJS_CCONTAINER_TESTS is [$(OBJS_CCONTAINER_TESTS)])
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))

unit_test :: test_clist_generic test_clist_cstring test_cvector_generic test_cvector_cstring test_cvector_struct_complex

# static library linkage is done as any other object file
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_clist_generic @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o $(MODDIR_CCONTAINER)/value.o -L $(LIB_CMOCKA) -lcmocka

# here test with public API only(don't include impl *c), must provide *.o dependencies
# or use libccontainer
OBJ_T_CLIST_CSTRING = $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o $(MODDIR_CCONTAINER)/clist_cstring.o \
						$(MODDIR_CCONTAINER)/clist_generic.o  $(MODDIR_CCONTAINER)/cvector_generic.o \
						$(MODDIR_CCONTAINER)/ccontainer_utils.o $(MODDIR_CCONTAINER)/value_cstring.o $(MODDIR_CCONTAINER)/value.o \
					
test_clist_cstring: $(OBJ_T_CLIST_CSTRING)
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(OBJ_T_CLIST_CSTRING) -L $(LIB_CMOCKA) -lcmocka

test_cvector_generic : $(MODDIR_CCONTAINER_TESTS)/test_cvector_generic.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_generic @ :    $@"  #
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

test_cvector_cstring : $(MODDIR_CCONTAINER_TESTS)/test_cvector_cstring.o $(MODDIR_CCONTAINER)/cvector_cstring.o $(MODDIR_CCONTAINER)/cvector_generic.o \
		$(MODDIR_CCONTAINER)/value_cstring.o $(MODDIR_CCONTAINER)/ccontainer_utils.o $(MODDIR_CCONTAINER)/clist_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

test_cvector_struct_complex : $(MODDIR_CCONTAINER_TESTS)/test_cvector_struct_complex.o $(MODDIR_CCONTAINER)/cvector_generic.o $(MODDIR_CCONTAINER)/value.o
	@echo "Building test_cvector_struct_complex @ :    $@"
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "clean unit_test ccontainer"
	rm -f $(OBJS_CCONTAINER_TESTS)
	rm -f test_clist_generic test_clist_cstring test_cvector_generic test_cvector_cstring test_cvector_struct_complex