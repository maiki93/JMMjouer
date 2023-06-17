#
# split games in sub directory

MODDIR_GAMES := jmmjouer\games

# add default_games.h /.c here ? $(MODDIR_GAMES)/default_games.o
# must be added to OBJS_GAME_LOADER in order to build fully the library
OBJS_GAMES += $(MODDIR_GAMES)/game_mastermind.obj $(MODDIR_GAMES)/default_games.obj
# added to all for no_lib, done by parent module
# OBJS_ALL_STATIC += $(OBJS_GAMES)

$(MODDIR_GAMES)/game_pendu.obj : $(MODDIR_GAMES)/game_pendu.c
	@echo "Build specific game_pendu in games : $@"
	$(CC) $(STD) /DwithLIB /Dshared_EXPORTS $(CFLAGS) /c $< /Fo"$(dir $@)"
#	$(CC) $(STD) $(CFLAGS) $(SHARED_EXPORT) /c $< /Fo"$(dir $@)"

# compile shared library to include at compile-time in main executable
# for victory / important to match the correct rule in module joueur
game_pendu.dll : $(MODDIR_GAMES)/game_pendu.obj $(MODDIR_JOUEUR)/score_game.obj
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:game_pendu_dll.lib /OUT:game_pendu.dll

game_pendu_dll.lib : game_pendu.dll

clean ::
	@echo "Clean module games $(MODDIR_GAMES)"
	del $(MODDIR_GAMES)\*.obj
	del game_pendu.dll, game_pendu_dll.lib, game_pendu_dll.exp