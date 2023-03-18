#
# split games in sub directory

MODDIR_GAMES := jmmjouer\games

$(MODDIR_GAMES)/game_pendu.obj : $(MODDIR_GAMES)/game_pendu.c
	@echo "Build specific game_pendu in games : $@"
	$(CC) $(STD) /DwithLIB /Dshared_EXPORTS $(CFLAGS) /c $< /Fo"$(dir $@)"
#	$(CC) $(STD) $(CFLAGS) $(SHARED_EXPORT) /c $< /Fo"$(dir $@)"

# compile shared library to include at compile-time in main executable
# for victory / important to match the correct rule in module joueur
game_pendu.dll : $(MODDIR_GAMES)/game_pendu.obj $(MODDIR_JOUEUR)/victory.obj
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:game_pendu_dll.lib /OUT:game_pendu.dll

game_pendu_dll.lib : game_pendu.dll

clean ::
	del $(MODDIR_GAMES)\*.obj
#	del game_pendu.dll, game_pendu_dll.lib, game_pendu.exp