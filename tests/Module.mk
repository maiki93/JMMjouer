## Define a module to compile the tests
# third version, make a general rule for all tests
#       todo, make info printing optional, use of function..
#
# second version :
# include *.c file directly in test (adviced with cmocka )
# => no more dependencies for compilation
#
# first :
# add option -Wno-implicit-function-declaration to suppress warning for "private function mm-algo" (not decalred in a header)
MODDIR_TESTS := tests

# specific compiler options for this directory only.
# if insert *.c file in in the test (adviced by cmocka), no need of it. keep as example
# OPTION_CC_TESTS = -Wno-implicit-function-declaration $(OPTION_CC)
OPTION_CC_TESTS = $(OPTION_CC)
# $(info $$OPTION_CC_TESTS is [$(OPTION_CC_TESTS)])

# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wno-implicit
SRCS_TESTS := $(wildcard $(MODDIR_TESTS)/*.c) # keep full path tests/test_mastermind.c
# $(info $$SRCS_TESTS is [$(SRCS_TESTS)])
OBJS_TESTS := $(patsubst %.c, %.o, $(SRCS_TESTS)) # keep full path tests/test_mastermind.o
# $(info $$OBJS_TESTS is [$(OBJS_TESTS)] )
# take away extension
EXE_TESTS := $(patsubst %.c, %, $(SRCS_TESTS)) # keep full path tests/test_mastermind.o
# $(info $$EXE_TESTS is [$(EXE_TESTS)] )


#! yes, working as expected, override generic rules for this directory
$(OBJS_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in tests:   $@"
	$(CC) $(OPTION_CC_TESTS) -c $< -o $@

# everything is imported in root directory, use OBJS constructued in root
# this one dpendends on clear_screen to compile : utils.o
#test_mastermind :  $(MODDIR_TESTS)/test_mastermind.o utils.o
#	@echo "Building unit tests :    $@"
#	$(CC) $(MODDIR_TESTS)/test_mastermind.o utils.o -l cmocka -o test_mastermind

# define the list of executable tests in unit_test target
unit_test:: $(EXE_TESTS)

# called only  for test_X, not sure why. trick use notdir to create executable in root/
%: %.o utils.o
	@echo "Building generic test_X @ :    $@"  # target name
#	@echo "Building generic test_X < :    $<"  # test_X.o
#	@echo "Building generic test_X ^ :    $^"  # test_X.o utils.o (ok for compil)
	$(CC) -o $(notdir $@) $^ -l cmocka

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	rm -f $(OBJS_TESTS)
# rm $(MODDIR_TESTS)/*.o
	rm -f $(notdir $(EXE_TESTS))
