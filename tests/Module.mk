## Define a module to compile the tests
# add option -Wno-implicit-function-declaration to suppress warning for "private function mm-algo" (not decalred in a header)
MODDIR_TESTS := tests

# specific compiler options for this directory only.
OPTION_CC_TESTS = -Wno-implicit-function-declaration $(OPTION_CC)
# $(info $$OPTION_CC_TESTS is [$(OPTION_CC_TESTS)])

# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wnoèimplicit
SRCS_TESTS := $(wildcard $(MODDIR_TESTS)/*.c) # keep full path tests/test_mastermind.c
# $(info $$SRCS_TESTS is [$(SRCS_TESTS)])
OBJS_TESTS := $(patsubst %.c, %.o, $(SRCS_TESTS)) # keep full path tests/test_mastermind.o
# $(info $$OBJS_TESTS is [$(OBJS_TESTS)] )

#! yes, working as expected, override generic rules for this directory
$(OBJS_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in tests:   $@"
	$(CC) $(OPTION_CC_TESTS) -c $< -o $@

# everything is imported in root directory, use OBJS constructued in root 
test_mastermind :  $(OBJS) $(MODDIR_TESTS)/test_mastermind.o
	@echo "Building unit tests :    $@"
	$(CC) $(MODDIR_TESTS)/test_mastermind.o $(OBJS) -l cmocka -o test_mastermind

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	rm -f $(MODDIR_TESTS)/*.o
	rm -f test_mastermind
