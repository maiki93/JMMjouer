## Define a module to compile the tests
# third version, make a general rule for all tests. just need to create a new test_X.c file in the directory
#                recompile only minimimum, and for any changes (modif test_X.c or delete test_X executable)
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
# take away extension (patsubst), and all directories  (notdir)
EXE_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_TESTS) ))
$(info $$EXE_TESTS_BIN is [$(EXE_TESTS_BIN)] ) # list test_mastermind, test_historique

# override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in tests:   $@"
	$(CC) $(OPTION_CC_TESTS) -c $< -o $@

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_TESTS_BIN)

# check existence of root/test_X executable, dependence in tests/text_x.o, 
$(EXE_TESTS_BIN): %: $(MODDIR_TESTS)/%.o utils.o
	@echo "Building generic test_X @ :    $@"  # target name
#	@echo "Building generic test_X < :    $<"  # test_X.o
#	@echo "Building generic test_X ^ :    $^"  # test_X.o utils.o (ok for compil)
	$(CC) -o $@ $^ -l cmocka

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	rm -f $(OBJS_TESTS)
	rm -f $(EXE_TESTS_BIN)
