#
# split games in sub directory

MODDIR_GAMES := jmmjouer/games

# add default_games.h /.c here ? $(MODDIR_GAMES)/default_games.o
# must be added to OBJS_GAME_LOADER in order to build fully the library
OBJS_GAMES += $(MODDIR_GAMES)/game_mastermind.o $(MODDIR_GAMES)/default_games.o

# added to all for no_lib, done by module parent 
# OBJS_ALL_STATIC += $(OBJS_GAMES)

# compile shared library to include at compile-time in main executable
# for victory / important to match the correct rule in module joueur
libgame_pendu.dll : $(MODDIR_GAMES)/game_pendu.o $(MODDIR_JOUEUR)/score_game.o
	$(CC) -shared $(CFLAGS) -o $@ $^ -Wl,--out-implib,libgame_pendu_dll.lib

# *lib also working ... ??
libgame_pendu_dll.lib : game_pendu.dll

clean ::
#	rm -f $(OBJS_GAMES)
	rm -f $(MODDIR_GAMES)/game_pendu.o
	rm -f libgame_pendu.dll libgame_pendu_dll.lib libgame_pendu.a