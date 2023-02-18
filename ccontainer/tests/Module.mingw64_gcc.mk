
CFLAGS_TESTS = -g3 -W -Wall -fPIC -Wunused -Wextra -pedantic -Wstrict-overflow=5 -Wno-unused-local-typedefs -fno-inline
# need c99 standard to use cmocka
# gnu implements inline -std=gnu89 or -fgnu89-inline but does not seem to work. Other errors appears with c89
STD_TESTS = -std=c99

INCLUDE_CMOCKA = C:\dev\cmocka_local_mingw\include
LIB_CMOCKA = C:\dev\cmocka_local_mingw\lib

$(info)
$(info == CCONTAINER UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))

SRCS_CCONTAINER_TESTS := $(wildcard $(MODDIR_CCONTAINER_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_CCONTAINER_TESTS) ])
OBJS_CCONTAINER_TESTS := $(patsubst %.c, %.o, $(SRCS_CCONTAINER_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])

#BIN_CCONTAINER_TESTS

# Override generic rules for this directory, all format tests/*.c *.o 
# missing cmocka include otherwise
$(OBJS_CCONTAINER_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in ccontainer tests:   $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I . -I $(INCLUDE_CMOCKA)

unit_test :: test_clist_generic test_clist_cstring

# static library linkage is done as any other object file
test_clist_generic: $(MODDIR_CCONTAINER_TESTS)/test_clist_generic.o libclogger.a
	@echo "Building test_clist_generic @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# here test with public API only(don't include impl *c), must provide *.o dependencies
test_clist_cstring: $(MODDIR_CCONTAINER_TESTS)/test_clist_cstring.o $(MODDIR_CCONTAINER)/clist_generic.o \
					$(MODDIR_CCONTAINER)/clist_cstring.o libclogger.a
	@echo "Building test_clist_cstring @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	rm -f $(OBJS_CCONTAINER_TESTS)
	rm -f test_clist_generic test_clist_cstring