################################## 
#
# Specific module to use mock with cmocka
# 	using mock may need different compilation steps / setup macro variable..
# 	-Wl,--wrap=func_to_mock
# 	    case for testing in an other implementation file than the tested function
#
# 	define WEAK_FOR_CMOCKA __atribute__((weak)) with -DUNIT_TEST_CMOCKA
# 		to apply to the original implementation file (not in normal release or other test)
# 		in this case the file must not be compiled in root, but here
#       case for game_mastermind.c
# 
#   could be easier to recompile all implementation with WEAK	
#  
###################################

# change name to specific module, may be can overrite ??
MODDIR_TESTS_MOCK := tests/test_mock

# specific compiler options for this directory only.
# if insert *.c file in in the test (adviced by cmocka), no need of it. keep as example
# OPTION_CC_TESTS = -Wno-implicit-function-declaration $(OPTION_CC)
# OPTION_CC_TESTS_MOCK = $(OPTION_CC)
OPTION_CC_TESTS_MOCK = $(filter-out -Wunused, $(OPTION_CC))
# $(info $$OPTION_CC_TESTS_MOCK is [$(OPTION_CC_TESTS)])

# could add linker option -lcmocka

# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wno-implicit
SRCS_TESTS_MOCK := $(wildcard $(MODDIR_TESTS_MOCK)/*.c) # keep full path tests/test_mastermind.c
 $(info $$SRCS_TESTS_MOCK is [$(SRCS_TESTS_MOCK)])
OBJS_TESTS_MOCK := $(patsubst %.c, %.o, $(SRCS_TESTS_MOCK)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS_MOCK is [$(OBJS_TESTS_MOCK)] )
# take away extension (patsubst), and all directories  (notdir)
EXE_TESTS_MOCK_BIN := $(notdir $(patsubst %.c, %, $(SRCS_TESTS_MOCK) ))
$(info $$EXE_TESTS_MOCK_BIN is [$(EXE_TESTS_MOCK_BIN)] ) # list test_mastermind, test_historique

## OBJS_TO_WEAKEN := $(MODDIR_TESTS_MOCK)/game_mastermind.o
## $(OBJS_TO_WEAKEN): %o: :%c

# want a local copy, becasue -D specific variable
$(MODDIR_TESTS_MOCK)/game_mastermind.o : game_mastermind.c
	@echo "Build *.o overriden rules for attr_weak in tests/test_mock:   $@"
	$(CC) -DUNIT_TEST_CMOCKA $(OPTION_CC_TESTS_MOCK) -c $< -o $@

# override generic rules for this directory, all format tests/*.c *.o 
# can desactivate many warning  in the siganture of mock functions
$(OBJS_TESTS_MOCK): %.o: %.c
	@echo "Build *.o overriden generic rules in tests/test_mock:   $@"
	$(CC) $(OPTION_CC_TESTS_MOCK) -Wno-unused-parameter -c $< -o $@

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_TESTS_MOCK_BIN)

# check existence of root/test_X executable, dependence in tests/text_x.o, 
# because of wrap argument,
test_games_mastermind_mock : $(MODDIR_TESTS_MOCK)/test_games_mastermind_mock.o $(MODDIR_TESTS_MOCK)/game_mastermind.o utils.o historique.o 
	@echo "Building generic test_mock_X @ :    $@"  # target name
	@echo "Building generic test_mock_X < :    $<"  # test_X.o
	@echo "Building generic test_mock_X ^ :    $^"  # test_X.o utils.o (ok for compil)
	$(CC) -o $@ $^ -Wl,--wrap=ask_yesno_question  -lcmocka 

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS_MOCK) "
	rm -f $(OBJS_TESTS_MOCK)
	rm -f $(EXE_TESTS_MOCK_BIN)
	rm -f $(MODDIR_TESTS_MOCK)/game_mastermind.o
