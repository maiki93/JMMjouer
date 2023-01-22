## Define a module to compile the tests
#
# version for msvc compilation
#
MODDIR_TESTS := tests

# Cmocka need c99 at minimum ? msvc no such standard, keep default (?)
# specific compiler options for this directory only.
# if insert *.c file in in the test (adviced by cmocka), no need of it. keep as example
# CFLAGS_TESTS = -Wno-implicit-function-declaration $(CFLAGS)
CFLAGS_TESTS = $(CFLAGS)
# -fno-inline
#CFLAGS_TESTS = -g -g3 -W -Wall -fPIC -Wunused -Wextra -pedantic -Wstrict-overflow=5 -Wno-unused-local-typedefs -fno-inline
# gnu implements inline -std=gnu89 or -fgnu89-inline but does not seem to work. Other errors appears with c89
STD_TESTS = "" 
#/std:c99

INCLUDE_CMOCKA = C:\dev\cmocka_local_msvc\include
LIB_CMOCKA = C:\dev\cmocka_local_msvc\lib

$(info $$CFLAGS_TESTS is [ $(CFLAGS_TESTS) ])

# it was a wrong good idea to try automatise the test generation, or need to split in some way
# specific targets in this directory, only OBJS_TESTS is used. list of test_X.c files compiled with a specific option -Wno-implicit
SRCS_TESTS := $(wildcard $(MODDIR_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_TESTS) ])
OBJS_TESTS := $(patsubst %.c, %.obj, $(SRCS_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_TESTS)])
# take away extension (patsubst), and all directories  (notdir)
EXE_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_TESTS) ))
$(info $$EXE_TESTS_BIN is [$(EXE_TESTS_BIN)] ) # list test_mastermind, test_historique

# Override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_TESTS): %.obj: %.c
	@echo "Build *.o overriden generic rules in tests:   $@"
	$(CC) $(CFLAGS) /c $< /I"$(INCLUDE_CMOCKA)" /Fo"$(MODDIR_TESTS)\\"

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_TESTS_BIN)

##### specific rule for tests, or shoud be the rule by default (simple one)
# split internal / (with cpp include ) ?

# test_plugin_manager, make sense could be appart of jmmjouer
# warning : overidde recipe if the One general rule used
test_plugin_manager: $(MODDIR_TESTS)/test_plugin_manager.o clogger.o
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<"  		# test_X.o
	@echo "Building test_plugin_manager ^ :    $^"       # test_X.o utils.o (ok for compil)
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

test_file_record: $(MODDIR_TESTS)/test_file_record.obj irecord.obj cmap_game_victories.obj clist_generic.obj clogger.obj person.obj joueur.obj
	@echo "Building test_file_record @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# need for game_X,  so need for utils
test_game_loader: $(MODDIR_TESTS)/test_game_loader.o cmap_ptrf_game.o clist_generic.o clist_cstring.o clogger.o plugin_manager.o utils.o utils_file.o game_pendu.o game_mastermind.o
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# very basic, general
test_clist_generic: $(MODDIR_TESTS)/test_clist_generic.obj clogger.obj
	echo "Building test_clist_generic @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# here test with public API only(don't include impl *c), must provide *.o dependencies
test_clist_cstring: $(MODDIR_TESTS)/test_clist_cstring.obj clist_cstring.obj clist_generic.obj clogger.obj
	@echo "Building test_game_loader @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# cmap_game_victories.o not needed include *.c implementation
test_cmap_game_victories: $(MODDIR_TESTS)/test_cmap_game_victories.obj clist_generic.obj clogger.obj
	@echo "Building test_game_victories @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# same as cmap_game_victories
test_cmap_game_ptrf: $(MODDIR_TESTS)/test_cmap_game_ptrf.obj clist_generic.obj clist_cstring.obj clogger.obj
	@echo "Building test_game_ptrf @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_mastermind: $(MODDIR_TESTS)/test_mastermind.obj utils.obj
	@echo "Building test_game_loader @ :    $@"  # target name
	$(LINK) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?
	
### Seems convenient to have a simple rule, but need all dependencies for each 
#	and create double declaration (e.g. game_loader and plugin manager)
# Kepp for example but not flexible enough (or better rule to find)
# check existence of root/test_X executable, dependence in tests/text_x.o,
# test_game_loader.exe : SHOULD NOT  BE needed game_mastermind.o, include the cpp file directly
#$(EXE_TESTS_BIN): %: $(MODDIR_TESTS)/%.o utils.o utils_file.o clogger.o plugin_manager.o game_mastermind.o game_pendu.o
#	@echo "Building generic test_X @ :    $@"  # target name
#	@echo "Building generic test_X < :    $<"  # test_X.o
#	@echo "Building generic test_X ^ :    $^"  # test_X.o utils.o (ok for compil)
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka



# to compile game_loader, game dependencies 
# gcc -std=c99 -g -g3 -W -Wall -fPIC -Wunused -Wextra -pedantic -Wstrict-overflow=5 -Wno-unused-local-typedefs -fno-inline -c tests/test_game_loader.c -o tests/test_game_loader.o -I"C:\dev\cmocka_local_mingw\include" -L. -lpendu
# gcc -std=c99 -Werror -Wall -Wextra -pedantic -Wno-variadic-macros -Wno-strict-overflow -g -g3 -O0 -DJMMJ_DEBUG -o test_game_loader tests/test_game_loader.o utils.o game_mastermind.o -L"C:\dev\cmocka_local_mingw\lib" -lcmocka -L. -lpendu

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
	del $(OBJS_TESTS)
	del $(EXE_TESTS_BIN)
