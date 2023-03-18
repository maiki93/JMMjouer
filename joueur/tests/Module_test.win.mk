


SRCS_JOUEUR_TESTS := $(wildcard $(MODDIR_JOUEUR_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JOUEUR_TESTS := $(patsubst %.c, %.obj, $(SRCS_JOUEUR_TESTS)) # keep full path tests/test_mastermind.o

OBJS_ALL_TESTS += $(OBJS_JOUEUR_TESTS)

$(info == JOUEUR UNIT_TEST :  $(MODDIR_JOUEUR_TESTS) ==)
$(info $$SRCS_JOUEUR_TESTS is [ $(SRCS_JOUEUR_TESTS) ])
$(info $$OBJS_JOUEUR_TESTS is [$(OBJS_JOUEUR_TESTS)])

unit_test :: test_cmap_game_victories

# no exported lib when use of shared library for both
test_cmap_game_victories: $(MODDIR_JOUEUR_TESTS)/test_cmap_game_victories.obj $(IMPORT_LIB_CLOGGER) $(IMPORT_LIB_CCONTAINER)
	@echo "Building test_game_victories @ :    $@"  # target name
	@echo "Building test_game_victories ^ :    $^"  # all dep.
	@echo "Building test_game_victories ^ :    $?"  # all dep. newer than target
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

clean ::
	del $(MODDIR_JOUEUR_TESTS)\*.obj
	del test_cmap_game_victories.exe






