
MODDIR_JOUEUR := joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR = $(patsubst %.c, %.o, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

# define tests file
MODDIR_JOUEUR_TESTS = $(MODDIR_JOUEUR)/tests

$(info == JOUEUR : $(MODDIR_JOUEUR )==)
$(info $$SRCS_JOUEUR is [$(SRCS_JOUEUR)] )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

# include static library without special directive, normal dependencies
# no special flag, libclogger is required
libjoueur.dll : $(OBJS_JOUEUR) libccontainer libclogger
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_JOUEUR) -Wl,--out-implib,libjoueur_dll.a -L. -lccontainer -lclogger

libjoueur.a : $(OBJS_JOUEUR)
	ar rcs $@ $^
 
ifneq ($(findstring libjoueur,$(LIB_STATIC)),)
IMPORT_LIB_JOUEUR = joueur
libjoueur : libjoueur.a
else
IMPORT_LIB_JOUEUR = joueur_dll
libjoueur : libjoueur.dll
endif

# include module of tests
include $(patsubst %,%/Module_test.mingw64_gcc.mk,$(MODDIR_JOUEUR_TESTS))

############### UNIT TESTS #########################
## do not make a test Module for only a few function
#nit_test :: test_cmap_game_victories

# need the specific cflags_tests at compilation
#$(MODDIR_JOUEUR)/tests/test_cmap_game_victories.o : $(MODDIR_JOUEUR)/tests/test_cmap_game_victories.c
#	@echo "Build *.o overriden generic rules in joueur for tests :   $@"
#	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -c $< -o $@ -I. -I $(INCLUDE_CMOCKA)

# cmap_game_victories.o not needed include *.c implementation
#test_cmap_game_victories: $(MODDIR_JOUEUR)/tests/test_cmap_game_victories.o libccontainer.a libclogger.a
#	@echo "Building test_game_victories @ :    $@"  # target name
#	$(CC) $(STD_TESTS) $(CFLAGS) -o $@ $^ -L $(LIB_CMOCKA) -lcmocka

clean ::
	@echo "Clean module joueur"
	rm -f $(OBJS_JOUEUR)
#	rm -f $(MODDIR_JOUEUR)/tests/*.o
	rm -f libjoueur.dll libjoueur_dll.a libjoueur.a