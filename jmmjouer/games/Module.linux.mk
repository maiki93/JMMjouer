#
# split games in sub directory
#
# mastermind : know header and implementation at compilation : add to the object list file of game loader
#
# pendu : knwon only an interface (header), will be necessary to link at the compilation time
#
# morpion : loaded dynamically at runtime, never access to header
#		1. agree on a API : exported variables, function name and signature. load from game_loader(plugin mangnager)
#       2. made default executable function in lib, and register itself (through a singleton necessarly)

MODDIR_GAMES := jmmjouer/games

# add default_games.h /.c here ? $(MODDIR_GAMES)/default_games.o
# must be added to OBJS_GAME_LOADER in order to build fully the library
OBJS_GAMES += $(MODDIR_GAMES)/game_mastermind.o
# added to all for no_lib
OBJS_ALL_STATIC += $(OBJS_GAMES)

# compile shared library to include at compile-time in main executable
# for victory / important to match the correct rule in module joueur
libgame_pendu.so : $(MODDIR_GAMES)/game_pendu.o $(MODDIR_JOUEUR)/victory.o
	$(LINK) $(LFLAGS) -o $@ $^
#	$(CC) -shared $(CFLAGS) -o $@ $^ -Wl,--out-implib,game_pendu_dll.lib

libgame_pendu : libgame_pendu.so

clean ::
	rm -f $(MODDIR_GAMES)/*.o
	rm -f libgame_pendu.so