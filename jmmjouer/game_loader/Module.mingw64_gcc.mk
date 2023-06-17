
MODDIR_GAME_LOADER := jmmjouer/game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER = $(patsubst %.c, %.o, $(SRCS_GAME_LOADER))

# update global variable
# OBJS_ALL_STATIC += $(OBJS_GAME_LOADER)

$(info == GAME_LOADER : $(MODDIR_GAME_LOADER) ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

#OBJS_GAME_LOADER = $(MODDIR_GAME_LOADER)/cmap_ptrf_game.o $(MODDIR_GAME_LOADER)/plugin_manager.o \
#		$(MODDIR_GAME_LOADER)/game_pendu.o  $(MODDIR_GAME_LOADER)/game_mastermind.o \
#		$(MODDIR_GAME_LOADER)/game_loader.o

#$(info == GAME_LOADER after ==)
#$(info $$OBJS is $(OBJS) )
#$(info $$OBJS_GAME_LOADER is $(OBJS_GAME_LOADER) )

# undefined start_game_pendu, start_game_mastermind, for_files_with_extension
#  then by inclusion dependencies to utils.o clear_stdin()
#libgame_loader.dll : $(OBJS) libccontainer.a libclogger.a
#	$(CC) -shared $(CFLAGS) -o $@ $^  -Wl,--out-implib,libgame_loader.lib

# pass lots of dep added : 
# nm libgame_loader.dll return only 3 fields ? then missing plugin_manager function for linking ...
# -L. -ljoueur, only vicctory is enought
# -Wl,--export-all-symbols to export all, needed because inclusion of 1 ( declspec(dllexport)) in game ! 
#   only for PE file, windows
#libgame_loader.dll : $(OBJS_GM) utils_file.o utils.o joueur/victory.o game_pendu.o game_mastermind.o libccontainer.a libclogger.a 
#	$(CC) -shared $(CFLAGS) -Wl,--export-all-symbols -o $@ $^ -L. -lccontainer -Wl,--out-implib,libgame_loader_dll.lib


# fine with dll, but include lots of dependencies (and repeat in others)
# still better with games included here

##### LAST
# last : do not compile because undefined ref to start_game_pendu in default_games
#libgame_loader.dll : $(OBJS_GAME_LOADER) $(OBJS_GAMES) jmmjouer/utils_file.o jmmjouer/utils.o joueur/victory.o libjoueur libccontainer libclogger
#	@echo "Create shared library -- game_loader"

# like in liunx, libjoueur is not enought, missing clist_cstring modules
# include libccontainer.lib => multiple definition clist_gen_X free_value...
#	$(CC) -shared $(CFLAGS) -Wl,--export-all-symbols -o $@ $^ -Wl,--out-implib,libgame_loader_dll.a -L. -ljoueur_dll libccontainer.lib
# fine with shared lib. to compare linux. same with libccontainer.dll. -ljoueur_dll, victory enough

###### LAST
# last, need clogger. because --export-all-symbols ?
#	$(CC) -shared $(CFLAGS) -Wl,--export-all-symbols -o $@ $(OBJS_GAME_LOADER) $(OBJS_GAMES) jmmjouer/utils_file.o jmmjouer/utils.o joueur/victory.o -Wl,--out-implib,libgame_loader_dll.a -L. -l$(IMPORT_LIB_CCONTAINER) -l$(IMPORT_LIB_CLOGGER)

#libgame_loader.a : $(OBJS_GAME_LOADER) $(OBJS_GAMES)
#	@echo "Create static library -- game_loader"
#	ar rcs $@ $^
#
#ifneq ($(findstring libgame_loader,$(LIB_STATIC)),)
#IMPORT_LIB_GAME_LOADER = game_loader
#libgame_loader : libgame_loader.a
#else
#IMPORT_LIB_GAME_LOADER = game_loader_dll
#libgame_loader : libgame_loader.dll
#endif

##### compile shared library to include at compile-time
#libgame_pendu.dll : $(MODDIR_GAME_LOADER)/game_pendu.o $(MODDIR_GAME_LOADER)/game_pendu.h joueur/victory.o
#	$(CC) -shared $(CFLAGS) $^ -o $@ -Wl,--out-implib,libgame_pendu.lib

# include description for each module
include $(MODDIR_GAME_LOADER)/tests/Module_test.mingw64_gcc.mk

clean ::
	@echo "Clean module game_loader"
#	rm -f $(MODDIR_GAME_LOADER)/*.o
#	rm -f libgame_loader.dll libgame_loader_dll.a libgame_loader.a
