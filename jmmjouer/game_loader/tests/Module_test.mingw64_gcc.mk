
#defined here rather than in parent, yes better
MODDIR_GL_TESTS = $(MODDIR_GAME_LOADER)/tests

$(info)
$(info == GAME_LOADER UNIT_TEST ==)
$(info $$CFLAGS_TESTS is $(CFLAGS_TESTS))
$(info $$STD_TESTS is $(STD_TESTS) )
$(info $$MODDIR_GL_TESTS is $(MODDIR_GL_TESTS) ) 
$(info dir: $(dir $(lastword $(MAKEFILE_LIST))) )

# seems very convenient ! NO it is not working well, or badly used (rewritten)
# specific rule was not called, called the generic one
# currentDir := $(dir $(lastword $(MAKEFILE_LIST)))
# currentDir2 = game_loader/tests
#$(info $$currentDir is $(currentDir) )

# If UT are specialized, make sense again to use generic way in some case
SRCS_GL_TESTS := $(wildcard $(MODDIR_GL_TESTS)/*.c)
$(info $$SRCS_GL_TESTS is [ $(SRCS_GL_TESTS) ])
OBJS_GL_TESTS := $(patsubst %.c, %.o, $(SRCS_GL_TESTS)) 
$(info $$OBJS_GL_TESTS is [$(OBJS_GL_TESTS)])


# Override generic rules for this directory, all format tests/*.c *.o 
$(OBJS_GL_TESTS): %.o: %.c
	@echo "Build *.o overriden generic rules in game_loader tests :   $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I. -I $(INCLUDE_CMOCKA)

unit_test :: test_mastermind test_cmap_game_ptrf test_plugin_manager test_game_loader

####
test_mastermind : $(MODDIR_GL_TESTS)/test_mastermind.o utils.o joueur/victory.o
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# same as cmap_game_victories, order important clogger last
test_cmap_game_ptrf: $(MODDIR_GL_TESTS)/test_cmap_game_ptrf.o libccontainer.a libclogger.a
	@echo "Building test_game_ptrf @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# test_plugin_manager, make sense could be appart of jmmjouer
# warning : overidde recipe if the One general rule used
test_plugin_manager: $(MODDIR_GL_TESTS)/test_plugin_manager.o libclogger.a
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<"  # test_X.o
	@echo "Building test_plugin_manager ^ :    $^"  # test_X.o utils.o (ok for compil)
	@echo "Building test_plugin_manager ? :    $?"  # 
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# need for game_X,  so need for utils
test_game_loader: $(MODDIR_GL_TESTS)/test_game_loader.o utils.o utils_file.o joueur/victory.o libgame_loader.lib libccontainer.a libclogger.a
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
#	@rm -f $(OBJS_TESTS)
#	@echo "Clean test unit game_loader: $(currentDir)*.o"
#	@echo $(dir $(lastword $(MAKEFILE_LIST)))
#	rm -f $(currentDir)*.o
#	rm -f game_loader/tests/*.o
	rm -f $(MODDIR_GL_TESTS)/*.o