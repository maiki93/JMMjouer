
SRCS_JOUEUR_TESTS := $(wildcard $(MODDIR_JOUEUR_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JOUEUR_TESTS := $(patsubst %.c, %.o, $(SRCS_JOUEUR_TESTS)) # keep full path tests/test_mastermind.o

$(info == JOUEUR UNIT_TEST : $(MODDIR_JOUEUR_TESTS) ==)
$(info $$SRCS_TESTS is [ $(SRCS_JOUEUR_TESTS) ])
$(info $$OBJS_TESTS is [$(OBJS_JOUEUR_TESTS)])

OBJS_ALL_TESTS += $(OBJS_JOUEUR_TESTS)

unit_test :: test_joueur test_map_game_score test_ccontainer_joueur

test_joueur: $(MODDIR_JOUEUR_TESTS)/test_joueur.o $(MODDIR_JOUEUR)/person.o $(MODDIR_JOUEUR)/map_game_victories.o $(MODDIR_JOUEUR)/joueur.o libccontainer libclogger
	@echo "Building test_joueur @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JOUEUR_TESTS)/test_joueur.o $(MODDIR_JOUEUR)/person.o $(MODDIR_JOUEUR)/map_game_victories.o $(MODDIR_JOUEUR)/joueur.o -L . -lclogger -lccontainer -L $(LIB_CMOCKA) -lcmocka

test_map_game_score: $(MODDIR_JOUEUR_TESTS)/test_map_game_score.o libclogger libccontainer
	@echo "Building test_map_game_score @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JOUEUR_TESTS)/test_map_game_score.o -L . -lccontainer -lclogger -L $(LIB_CMOCKA) -lcmocka

test_ccontainer_joueur: $(MODDIR_JOUEUR_TESTS)/test_ccontainer_joueur.o $(MODDIR_JOUEUR)/person.o $(MODDIR_JOUEUR)/map_game_victories.o $(MODDIR_JOUEUR)/joueur.o $(MODDIR_JOUEUR)/adapter_ccontainer_joueur.o $(MODDIR_JOUEUR)/list_joueur.o $(MODDIR_JOUEUR)/vector_joueur.o libccontainer libclogger
	@echo "Building test_ccontainer_joueur @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JOUEUR_TESTS)/test_ccontainer_joueur.o $(MODDIR_JOUEUR)/person.o $(MODDIR_JOUEUR)/map_game_victories.o $(MODDIR_JOUEUR)/joueur.o $(MODDIR_JOUEUR)/adapter_ccontainer_joueur.o $(MODDIR_JOUEUR)/list_joueur.o $(MODDIR_JOUEUR)/vector_joueur.o -L . -lclogger -lccontainer -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean unit_test joueur"
	rm -f $(OBJS_JOUEUR_TESTS)
	rm -f test_joueur test_map_game_score test_ccontainer_joueur

