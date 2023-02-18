
MODDIR_GAME_LOADER := game_loader
MODULES_TESTS = $(MODDIR_GAME_LOADER)/tests
# seems necessary... currentDir bad idea
# no need here, child knows MODDIR_GAME_LOADER
# MODDIR_GL_TESTS = $(MODDIR_GAME_LOADER)/tests

$(info)
$(info == GAME_LOADER ==)
$(info $$CURDIR is $(CURDIR) )
$(info $$OBJS is $(OBJS) )

OBJS_GAME_LOADER = $(MODDIR_GAME_LOADER)/cmap_ptrf_game.o $(MODDIR_GAME_LOADER)/plugin_manager.o \
		$(MODDIR_GAME_LOADER)/game_pendu.o  $(MODDIR_GAME_LOADER)/game_mastermind.o \
		$(MODDIR_GAME_LOADER)/game_loader.o

$(info == GAME_LOADER after ==)
$(info $$OBJS is $(OBJS) )
$(info $$OBJS_GAME_LOADER is $(OBJS_GAME_LOADER) )

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
#libgame_loader.dll : $(OBJS_GAME_LOADER) utils_file.o utils.o joueur/victory.o libccontainer.a libclogger.a
#	$(CC) -shared $(CFLAGS) -Wl,--export-all-symbols -o $@ $^ -Wl,--out-implib,libgame_loader_dll.lib

libgame_loader.lib : $(OBJS_GAME_LOADER)
	ar rcs $@ $^

#libgame_loader.a : $(MODDIR_GAME_LOADER)/cmap_ptrf_game.o $(MODDIR_GAME_LOADER)/plugin_manager.o \
#					$(MODDIR_GAME_LOADER)/game_loader.o
#	ar rcs $@ $^

##### compile shared library to include at compile-time
libgame_pendu.dll : $(MODDIR_GAME_LOADER)/game_pendu.o $(MODDIR_GAME_LOADER)/game_pendu.h joueur/victory.o
	$(CC) -shared $(CFLAGS) $^ -o $@ -Wl,--out-implib,libgame_pendu.lib

# include description for each module
include $(patsubst %,%/Module.mingw64_gcc.mk,$(MODULES_TESTS))

# MAYBE Error MinGW64 gcc : default export all, excepti if ONE __declspec(dllexport)
# can compile manually
# gcc -shared -o libtoto.dll game_loader.o cmap_ptrf_game.o plugin_manager.o ../utils.o ../utils_file.o ../game_pendu.o ../game_mastermind.o -L.. -lccontainer -lclogger -ljoueur -Wl,--out-implib,libtoto.lib
clean ::
	rm -f $(MODDIR_GAME_LOADER)/*.o

