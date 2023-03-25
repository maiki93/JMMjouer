#
# split games in sub directory
MODDIR_GAMES := jmmjouer/games

# compile shared library to include at compile-time in main executable
# for victory / important to match the correct rule in module joueur
libgame_pendu.dll : $(MODDIR_GAMES)/game_pendu.o $(MODDIR_JOUEUR)/victory.o
	$(CC) -shared $(CFLAGS) -o $@ $^ -Wl,--out-implib,game_pendu_dll.lib

# *lib also working ... ??
libgame_pendu_dll.lib : game_pendu.dll

clean ::
	rm -f $(MODDIR_GAMES)/*.o
	rm -f libgame_pendu.dll libgame_pendu_dll.lib libgame_pendu.a