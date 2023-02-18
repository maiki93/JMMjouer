## Define a module to compile the tests
# fourth version :
#     generic rules to compile object file list from "test_X".c
#     specific for each executable tests, keep better track of dependencies
#     CMocka need c99 standard
#
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

# Cmocka need c99 at minimum ?
# specific compiler options for this directory only.
# if insert *.c file in in the test (adviced by cmocka), no need of it. keep as example
# CFLAGS_TESTS = -Wno-implicit-function-declaration $(CFLAGS)
#CFLAGS_TESTS = $(CFLAGS)
# -fno-inline
CFLAGS_TESTS = -g3 -W -Wall -fPIC -Wunused -Wextra -pedantic -Wstrict-overflow=5 -Wno-unused-local-typedefs -fno-inline -I.
# need c99 standard to use cmocka
# gnu implements inline -std=gnu89 or -fgnu89-inline but does not seem to work. Other errors appears with c89
STD_TESTS = -std=c99

INCLUDE_CMOCKA = C:\dev\cmocka_local_mingw\include
LIB_CMOCKA = C:\dev\cmocka_local_mingw\lib

$(info )
$(info == UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))

# it was a wrong good idea to try automatise the test generation, or need to split in some way
# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wno-implicit
SRCS_TESTS := $(wildcard $(MODDIR_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_TESTS) ])
OBJS_TESTS := $(patsubst %.c, %.o, $(SRCS_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])
# take away extension (patsubst), and all directories  (notdir)
EXE_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_TESTS) ))
$(info $$EXE_TESTS_BIN is [$(EXE_TESTS_BIN)] ) # list test_mastermind, test_historique

# Override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in tests:   $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I $(INCLUDE_CMOCKA)

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_TESTS_BIN)

##### specific rule for tests, or shoud be the rule by default (simple one)
# split internal / (with cpp include ) ?

OBJS_NO_ARCADE := $(filter-out arcade.o, $(OBJS))

test_arcade: $(MODDIR_TESTS)/test_arcade.o libclogger.a libccontainer.a libgame_loader.lib libjoueur_dll.lib $(OBJS_NO_ARCADE) 
	@echo "Building test_arcade @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka -L. -lrecord

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	rm -f $(OBJS_TESTS)
	rm -f $(EXE_TESTS_BIN)
