


SRCS_JOUEUR_TESTS := $(wildcard $(MODDIR_JOUEUR_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JOUEUR_TESTS := $(patsubst %.c, %.obj, $(SRCS_JOUEUR_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_JOUEUR_TESTS)

$(info == JOUEUR UNIT_TEST :  $(MODDIR_JOUEUR_TESTS) ==)
$(info $$SRCS_JOUEUR_TESTS is [ $(SRCS_JOUEUR_TESTS) ])
$(info $$OBJS_JOUEUR_TESTS is [$(OBJS_JOUEUR_TESTS)])

unit_test :: test_joueur test_map_game_score test_ccontainer_joueur

test_joueur: $(MODDIR_JOUEUR_TESTS)/test_joueur.obj $(MODDIR_JOUEUR)/user.obj $(MODDIR_JOUEUR)/score_game.obj \
		 $(MODDIR_JOUEUR)/map_game_score.obj $(MODDIR_JOUEUR)/joueur.obj $(IMPORT_LIB_CLOGGER) $(IMPORT_LIB_CCONTAINER)
	@echo "Building test_joueur @ :    $@"  # target name
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

# no exported lib when use of shared library for both
test_map_game_score: $(MODDIR_JOUEUR_TESTS)/test_map_game_score.obj $(MODDIR_JOUEUR)/score_game.obj $(IMPORT_LIB_CLOGGER) $(IMPORT_LIB_CCONTAINER)
	@echo "Building test_map_game_score @ :    $@"  # target name
	@echo "Building test_map_game_score ^ :    $^"  # all dep.
	@echo "Building test_map_game_score ? :    $?"  # all dep. newer than target
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

test_ccontainer_joueur: $(MODDIR_JOUEUR_TESTS)/test_ccontainer_joueur.obj $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CLOGGER) $(IMPORT_LIB_CCONTAINER)
	@echo "Building test_ccontainer_joueur @ :    $@"  # target name
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

clean ::
	del $(MODDIR_JOUEUR_TESTS)\*.obj
	del test_joueur.exe, test_joueur.*
	del test_map_game_score.exe, test_map_game_score.*
	del test_ccontainer_joueur.exe, test_ccontainer_joueur.* 






