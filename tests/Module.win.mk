## Define a module to compile the tests
#
# version for msvc compilation
#
MODDIR_TESTS := tests

# compile with /wd4100 or add #pragma warning(disable : 4100) : for unused parameter
# 4113 : parameter void **state missing in unit test function
#CFLAGS_TESTS = $(CFLAGS)  /wd4113
CFLAGS_TESTS = /nologo /W4 /D_CRT_SECURE_NO_WARNINGS /wd4113
# other variables taken for the main Makefile

## CMocka it is 64 bit compilation
INCLUDE_CMOCKA = C:\dev\cmocka_local_msvc\include
LIB_CMOCKA = C:\dev\cmocka_local_msvc\lib

ifeq ($(debug),yes)          # separator mandatory between ifeq and '('
# use specific code for the project, g3 to include MACRO definition as variables
	CFLAGS_TESTS += /Zi /DJMMJ_DEBUG
else
# 'best' optimization msvc
	CFLAGS_TESTS += /Ox
endif

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
	$(CC) /Zi $(CFLAGS_TESTS) /c $< /I"$(INCLUDE_CMOCKA)" /Fo"$(MODDIR_TESTS)\\"

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_TESTS_BIN)

# warning : overidde recipe if the One general rule used
test_plugin_manager: $(MODDIR_TESTS)/test_plugin_manager.obj clogger.obj
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<" 	# test_X.o
	@echo "Building test_plugin_manager ^ :    $^"  # test_X.o utils.o (ok for compil)
	@echo "Building test_plugin_manager ^ :    $?"  # 
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_file_record: $(MODDIR_TESTS)/test_file_record.obj irecord.obj \
					cmap_game_victories.obj clist_generic.obj clogger.obj person.obj joueur.obj
	@echo "Building test_file_record @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_game_loader: $(MODDIR_TESTS)/test_game_loader.obj cmap_ptrf_game.obj clist_generic.obj \
				clist_cstring.obj clogger.obj plugin_manager.obj utils.obj utils_file.obj \
				victory.obj game_pendu.obj game_mastermind.obj
	@echo "Building test_game_loader @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_clist_generic: $(MODDIR_TESTS)/test_clist_generic.obj clogger.obj
	echo "Building test_clist_generic @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# here test with public API only(don't include impl *c), must provide *.o dependencies
test_clist_cstring: $(MODDIR_TESTS)/test_clist_cstring.obj clist_cstring.obj clist_generic.obj clogger.obj
	@echo "Building test_game_loader @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_cmap_game_victories: $(MODDIR_TESTS)/test_cmap_game_victories.obj clist_generic.obj clogger.obj
	@echo "Building test_game_victories @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_cmap_game_ptrf: $(MODDIR_TESTS)/test_cmap_game_ptrf.obj clist_generic.obj clist_cstring.obj clogger.obj
	@echo "Building test_game_ptrf @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?


OBJS_NO_ARCADE := $(filter-out arcade.obj, $(OBJS))

test_arcade: $(MODDIR_TESTS)/test_arcade.obj $(OBJS_NO_ARCADE) game_pendu.obj
	@echo "Building test_arcade @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

test_mastermind: $(MODDIR_TESTS)/test_mastermind.obj utils.obj victory.obj
	@echo "Building test_mastermind @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

# use only interface header in test
test_clogger: $(MODDIR_TESTS)/test_clogger.obj clogger.obj
	@echo "Building test_clogger @ :    $@"  # target name
	$(LINK) $(LFLAGS) /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?

clean::
	echo "Clean unit tests in MODDIR_C = $(MODDIR_TESTS) "
#	del $(EXE_TESTS_BIN) miss *.exe suffix
	del test_*.exe, test_*ilk, test_*pdb
	del $(MODDIR_TESTS)\*.obj $(MODDIR_TESTS)\*.pdb $(MODDIR_TESTS)\*.ilk

