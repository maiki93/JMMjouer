
SRCS_JOUEUR_TESTS := $(wildcard $(MODDIR_JOUEUR_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JOUEUR_TESTS := $(patsubst %.c, %.o, $(SRCS_JOUEUR_TESTS)) # keep full path tests/test_mastermind.o

$(info == JOUEUR UNIT_TEST : $(MODDIR_JOUEUR_TESTS) ==)
$(info $$SRCS_TESTS is [ $(SRCS_JOUEUR_TESTS) ])
$(info $$OBJS_TESTS is [$(OBJS_JOUEUR_TESTS)])

OBJS_ALL_TESTS += $(OBJS_JOUEUR_TESTS)

unit_test :: test_cmap_game_victories

test_cmap_game_victories: $(MODDIR_JOUEUR_TESTS)/test_cmap_game_victories.o libclogger libccontainer
	@echo "Building test_cmap_game_victories @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JOUEUR_TESTS)/test_cmap_game_victories.o -L . -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean unit_test joueur"
	rm -f $(OBJS_JOUEUR_TESTS)
	rm -f test_cmap_game_victories

