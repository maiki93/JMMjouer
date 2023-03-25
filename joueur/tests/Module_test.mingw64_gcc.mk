# copy of linux makefile

SRCS_JOUEUR_TESTS := $(wildcard $(MODDIR_JOUEUR_TESTS)/*.c)
OBJS_JOUEUR_TESTS := $(patsubst %.c, %.o, $(SRCS_JOUEUR_TESTS))

OBJS_ALL_TESTS += $(OBJS_JOUEUR_TESTS)

$(info == JOUEUR UNIT_TEST : $(MODDIR_JOUEUR_TESTS) ==)
$(info $$SRCS_TESTS is [ $(SRCS_JOUEUR_TESTS) ])
$(info $$OBJS_TESTS is [$(OBJS_JOUEUR_TESTS)])

unit_test :: test_cmap_game_victories

# no rule to make libccontainer.a with -lccontainer
test_cmap_game_victories: $(MODDIR_JOUEUR_TESTS)/test_cmap_game_victories.o libclogger libccontainer
	@echo "Building test_cmap_game_victories @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JOUEUR_TESTS)/test_cmap_game_victories.o -L. -l$(IMPORT_LIB_CCONTAINER) -l$(IMPORT_LIB_CLOGGER) -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean unit_test joueur"
	rm -f $(OBJS_JOUEUR_TESTS)
	rm -f test_cmap_game_victories

