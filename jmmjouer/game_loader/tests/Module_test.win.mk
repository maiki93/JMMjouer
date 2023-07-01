
#defined here rather than in parent, yes better, sure ?
# should work !  $(dir $@)
MODDIR_GL_TESTS = $(MODDIR_GAME_LOADER)\tests

SRCS_GL_TESTS := $(wildcard $(MODDIR_GL_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_GL_TESTS := $(patsubst %.c, %.obj, $(SRCS_GL_TESTS)) # keep full path tests/test_mastermind.o

$(info == GAME_LOADER UNIT_TEST : $(MODDIR_GL_TESTS) ==)
$(info $$SRCS_GL_TESTS is [ $(SRCS_GL_TESTS) ])
$(info $$OBJS_GL_TESTS is [$(OBJS_GL_TESTS)])
$(info dir: $(dir $(lastword $(MAKEFILE_LIST))) )

# not used to keep for later use 
# take away extension (patsubst), and all directories  (notdir)
#EXE_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_TESTS) ))
#$(info $$EXE_TESTS_BIN is [$(EXE_TESTS_BIN)] ) # list test_mastermind, test_historique


#OBJS_ALL_TESTS += $(OBJS_GL_TESTS)

# Override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_GL_TESTS): %.obj: %.c
	@echo "Build *.o overriden rules tests game_loader:   $@"
	@echo "files :   $^"
	@echo "dir @ : $(dir $@)"
# No flag, no library libjouer => still ib exported (but not warning 4217)
#	$(CC) /Zi $(CFLAGS_TESTS) /c $< /I. /I"$(INCLUDE_CMOCKA)" /Fo"$(dir $@)"
# without use of library libjoueur
# 	LINK : warning LNK4217: victory_init exported, and imported by test in the SAME object file
# with use of library libjoueur : both correct
#	$(CC) /Zi $(CFLAGS_TESTS) /DwithLIB /c $< /I. /I"$(INCLUDE_CMOCKA)" /Fo"$(dir $@)"
	$(CC) /Zi $(CFLAGS_TESTS) /c $< /I. /I"$(INCLUDE_CMOCKA)" /Fo"$(dir $@)"

unit_test :: test_mastermind test_map_game_ptrf test_plugin_manager test_game_loader

# include implementation game_mastermind
# if use joueur/victory.obj => always exported function (dllexport) in compilation
#test_mastermind : $(MODDIR_GL_TESTS)/test_mastermind.obj jmmjouer/utils.obj joueur/victory.obj
# all dependent libraries to build
#test_mastermind : $(MODDIR_GL_TESTS)/test_mastermind.obj jmmjouer/utils.obj $(IMPORT_LIB_JOUEUR)
# compile even with WITH_LIB=0
test_mastermind : $(MODDIR_GL_TESTS)/test_mastermind.obj jmmjouer/utils.obj joueur/score_game.obj
	@echo "Building test_mastermind @ :    $@"  # target name
	@echo "arg ? :    $?"
#	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

# $(IMPORT_LIB_CLOGGER) not needed , not used or because already in CCONTAINER (compiled clogger static seems the case)?
test_map_game_ptrf: $(MODDIR_GL_TESTS)/test_map_game_ptrf.obj $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Building test_cmap_game_ptrf @ :    $@"  # target name
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

# same with clogger, need library 
# or do not use import from doc (but still *.lib generated in both cases), just avoid the warning
test_plugin_manager: $(MODDIR_GL_TESTS)/test_plugin_manager.obj $(IMPORT_LIB_CLOGGER)
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<" 	# test_X.o
	@echo "Building test_plugin_manager ^ :    $^"  # test_X.o utils.o (ok for compil)
	@echo "Building test_plugin_manager ^ :    $?"  # 
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib


# libjoueur does not allow to get libcontainer function ? but ok for clogger ?
test_game_loader: $(MODDIR_GL_TESTS)/test_game_loader.obj $(MODDIR_GAME_LOADER)/map_game_ptrf.obj \
				$(MODDIR_GAME_LOADER)/plugin_manager.obj $(MODDIR_JMMJOUER)/utils_file.obj  $(MODDIR_JMMJOUER)/utils.obj \
				$(MODDIR_GAMES)/default_games.obj $(MODDIR_GAMES)/game_mastermind.obj \
				$(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CLOGGER) game_pendu_dll.lib
	@echo "Building test_game_loader @ :    $@"  # target name
#	$(LINK) $(LFLAGS) /NOIMPLIB /NOEXP /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?
	$(LINK) $(LFLAGS) $?  /LIBPATH:$(LIB_CMOCKA) cmocka.lib

clean ::
	del $(MODDIR_GL_TESTS)\*.obj
	del test_mastermind.exe, test_plugin_manager.exe, test_map_game_ptrf.exe, test_game_loader.exe

