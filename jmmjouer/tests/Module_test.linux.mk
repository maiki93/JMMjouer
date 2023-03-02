
$(info)
$(info == JMMJOUER CORE UNIT_TEST ==)
$(info $$MODDIR_JMMJOUER_TESTS is $(MODDIR_JMMJOUER_TESTS))


SRCS_JMMJOUER_TESTS := $(wildcard $(MODDIR_JMMJOUER_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_JMMJOUER_TESTS) ])
OBJS_JMMJOUER_TESTS := $(patsubst %.c, %.o, $(SRCS_JMMJOUER_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_JMMJOUER_TESTS)])

OBJS_ALL_TESTS += $(OBJS_JMMJOUER_TESTS)

# Cmocka need c99 at minimum ?
# specific compiler options for this directory only.
# if insert *.c file in in the test (adviced by cmocka), no need of it. keep as example
# CFLAGS_TESTS = -Wno-implicit-function-declaration $(CFLAGS)
#CFLAGS_TESTS = $(CFLAGS)
# -fno-inline
# -g3 : all debug information + MACRO (-g0 : no debug ), -g default 2
CFLAGS_TESTS = -g3 -Wall -fPIC -Wextra -pedantic -Wstrict-overflow=5 -Wno-unused-local-typedefs -fno-inline
# need c99 standard to use cmocka
# gnu implements inline -std=gnu89 or -fgnu89-inline but does not seem to work. Other errors appears with c89
STD_TESTS = -std=c99

## Ubuntu, global installation with package manager
INCLUDE_CMOCKA = C:\dev\cmocka_local_mingw\include
LIB_CMOCKA = C:\dev\cmocka_local_mingw\lib

$(info )
$(info == UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))

# it was a wrong good idea to try automatise the test generation, or need to split in some way
# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wno-implicit
SRCS_JMMJOUER_TESTS := $(wildcard $(MODDIR_JMMJOUER_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_JMMJOUER_TESTS is [ $(SRCS_JMMJOUER_TESTS) ])
OBJS_JMMJOUER_TESTS := $(patsubst %.c, %.o, $(SRCS_JMMJOUER_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_JMMJOUER_TESTS is [$(OBJS_JMMJOUER_TESTS)])

# take away extension (patsubst), and all directories  (notdir)
EXE_JMMJOUER_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_JMMJOUER_TESTS) ))
$(info $$EXE_JMMJOUER_TESTS_BIN is [$(EXE_JMMJOUER_TESTS_BIN)] ) # list test_mastermind, test_historique

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_JMMJOUER_TESTS_BIN)

OBJS_NO_ARCADE := $(filter-out $(MODDIR_JMMJOUER)/arcade.o, $(OBJS_JMMJOUER))

test_arcade: $(MODDIR_JMMJOUER_TESTS)/test_arcade.o $(OBJS_NO_ARCADE) librecord libgame_loader libjoueur libccontainer
	@echo "Building test_arcade @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $(MODDIR_JMMJOUER_TESTS)/test_arcade.o $(OBJS_NO_ARCADE) -L . -lrecord -lgame_loader -ljoueur -lccontainer  -L $(LIB_CMOCKA) -lcmocka

#test_mastermind: $(MODDIR_TESTS)/test_mastermind.o utils.o victory.o
#	@echo "Building test_mastermind @ :    $@"  # target name
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka
	
### Seems convenient to have a simple rule, but need all dependencies for each 
#	and create double declaration (e.g. game_loader and plugin manager)
# Keep for example but not flexible enough (or better rule to find)
# check existence of root/test_X executable, dependence in tests/text_x.o,
# test_game_loader.exe : SHOULD NOT  BE needed game_mastermind.o, include the cpp file directly
#$(EXE_TESTS_BIN): %: $(MODDIR_TESTS)/%.o utils.o utils_file.o clogger.o plugin_manager.o game_mastermind.o game_pendu.o
#	@echo "Building generic test_X @ :    $@"  # target name
#	@echo "Building generic test_X < :    $<"  # test_X.o
#	@echo "Building generic test_X ^ :    $^"  # test_X.o utils.o (ok for compil)
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	rm -f $(OBJS_JMMJOUER_TESTS)
	rm -f test_arcade
#	rm -f $(EXE_JMMJOUER_TESTS_BIN)
