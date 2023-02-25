

MODDIR_JOUEUR := joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR = $(patsubst %.c, %.o, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

$(info == JOUEUR ==)
$(info $$CURDIR is $(CURDIR) )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

# include static library without special directive, normal dependencies
libjoueur.dll : $(OBJS_JOUEUR) libccontainer.lib libclogger.lib
	$(CC) -shared $(CFLAGS) -o $@ $^  -Wl,--out-implib,libjoueur_dll.lib

############### UNIT TESTS #########################
## do not make a test Module for only a few function
unit_test :: test_cmap_game_victories

# need the specific cflags_tests at compilation
$(MODDIR_JOUEUR)/tests/test_cmap_game_victories.o : $(MODDIR_JOUEUR)/tests/test_cmap_game_victories.c
	@echo "Build *.o overriden generic rules in joueur for tests :   $@"
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I. -I $(INCLUDE_CMOCKA)

# cmap_game_victories.o not needed include *.c implementation
test_cmap_game_victories: $(MODDIR_JOUEUR)/tests/test_cmap_game_victories.o libccontainer.a libclogger.a
	@echo "Building test_game_victories @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	rm -f $(MODDIR_JOUEUR)/*.o
	rm -f $(MODDIR_JOUEUR)/tests/*.o
	rm -f libjoueur.dll