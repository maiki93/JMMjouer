
#defined here rather than in parent, yes better
MODDIR_GL_TESTS = $(MODDIR_GAME_LOADER)/tests

# If UT are specialized, make sense again to use generic way in some case
SRCS_GL_TESTS := $(wildcard $(MODDIR_GL_TESTS)/*.c)
OBJS_GL_TESTS := $(patsubst %.c, %.o, $(SRCS_GL_TESTS))
EXE_GL_TESTS := $(notdir $(patsubst %.c, %, $(SRCS_GL_TESTS) ))

OBJS_ALL_TESTS += $(OBJS_GL_TESTS)

$(info == GAME_LOADER UNIT_TEST : $(MODDIR_GL_TESTS) ==)
$(info $$SRCS_GL_TESTS is [ $(SRCS_GL_TESTS) ] )
$(info $$OBJS_GL_TESTS is [$(OBJS_GL_TESTS)] )
$(info $$EXE_GL_TESTS is [$(EXE_GL_TESTS)] )
$(info dir: $(dir $(lastword $(MAKEFILE_LIST))) )

unit_test :: $(EXE_GL_TESTS)

####
test_mastermind : $(MODDIR_GL_TESTS)/test_mastermind.o jmmjouer/utils.o joueur/score_game.o
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# same as map_game_score, order important clogger last
test_map_game_ptrf: $(MODDIR_GL_TESTS)/test_map_game_ptrf.o libccontainer libclogger
	@echo "Building test_game_ptrf @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $< -L . -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka 

# test_plugin_manager, make sense could be appart of jmmjouer
# warning : overidde recipe if the One general rule used
test_plugin_manager: $(MODDIR_GL_TESTS)/test_plugin_manager.o libclogger
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<"  # test_X.o
	@echo "Building test_plugin_manager ^ :    $^"  # test_X.o utils.o (ok for compil)
	@echo "Building test_plugin_manager ? :    $?"  # 
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $< -L. -lclogger -L $(LIB_CMOCKA) -lcmocka

# need for game_X,  so need for utils
test_game_loader: $(MODDIR_GL_TESTS)/test_game_loader.o jmmjouer/utils.o jmmjouer/utils_file.o joueur/score_game.o $(OBJS_GAMES) jmmjouer/game_loader/plugin_manager.o jmmjouer/game_loader/map_game_ptrf.o libccontainer libclogger libgame_pendu.dll
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@  $(MODDIR_GL_TESTS)/test_game_loader.o jmmjouer/utils.o jmmjouer/utils_file.o joueur/score_game.o $(OBJS_GAMES) jmmjouer/game_loader/plugin_manager.o jmmjouer/game_loader/map_game_ptrf.o -L. -l$(IMPORT_LIB_CLOGGER) -l$(IMPORT_LIB_CCONTAINER) -lgame_pendu_dll -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean test unit game_loader: $(MODDIR_GL_TESTS)"
#	rm -f $(MODDIR_GL_TESTS)/*.o
	rm -f $(OBJS_GL_TESTS)
	rm -f $(EXE_GL_TESTS)
