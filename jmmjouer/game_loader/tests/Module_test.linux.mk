
$(info)
$(info == GAME_LOADER UNIT_TEST ==)
$(info $$MODDIR_GAME_LOADER_TESTS is $(MODDIR_GAME_LOADER_TESTS))


SRCS_GAME_LOADER_TESTS := $(wildcard $(MODDIR_GAME_LOADER_TESTS)/*.c) # keep full path tests/test_mastermind.c
$(info $$SRCS_TESTS is [ $(SRCS_GAME_LOADER_TESTS) ])
OBJS_GAME_LOADER_TESTS := $(patsubst %.c, %.o, $(SRCS_GAME_LOADER_TESTS)) # keep full path tests/test_mastermind.o
$(info $$OBJS_TESTS is [$(OBJS_GAME_LOADER_TESTS)])

OBJS_ALL_TESTS += $(OBJS_GAME_LOADER_TESTS)

unit_test :: test_cmap_game_ptrf test_plugin_manager \
		     test_game_loader test_mastermind

test_mastermind : $(MODDIR_GAME_LOADER_TESTS)/test_mastermind.o utils.o joueur/victory.o
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# same as cmap_game_victories, order important clogger last
test_cmap_game_ptrf: $(MODDIR_GAME_LOADER_TESTS)/test_cmap_game_ptrf.o libccontainer.a libclogger.a
	@echo "Building test_game_ptrf @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

# test_plugin_manager, make sense could be appart of jmmjouer
# warning : overidde recipe if the One general rule used
test_plugin_manager: $(MODDIR_GAME_LOADER_TESTS)/test_plugin_manager.o libclogger.a
	@echo "Building test_plugin_manager @ :    $@"  # target name
	@echo "Building test_plugin_manager < :    $<"  # test_X.o
	@echo "Building test_plugin_manager ^ :    $^"  # test_X.o utils.o (ok for compil)
	@echo "Building test_plugin_manager ? :    $?"  # 
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

test_game_loader: $(MODDIR_GAME_LOADER_TESTS)/test_game_loader.o utils.o utils_file.o joueur/victory.o libgame_loader libccontainer libclogger
	@echo "Building test_game_loader @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_GAME_LOADER_TESTS)/test_game_loader.o utils.o utils_file.o joueur/victory.o -L . -lgame_loader -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

clean ::
	rm -f $(OBJS_GAME_LOADER_TESTS)
	rm -f test_game_loader test_cmap_game_ptrf test_plugin_manager test_mastermind
