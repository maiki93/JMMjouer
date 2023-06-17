
MODDIR_SRC_PLUGINS := src_plugins

OBJS_SRC_PLUGINS := $(MODDIR_SRC_PLUGINS)/game_morpion.obj

$(info == SRC_PLUGIN : $(MODDIR_SRC_PLUGINS) ==)
$(info $$OBJS_SRC_PLUGIN_MORP is [$(OBJS_SRC_PLUGINS)] )

# independent of other make config, always shared dlibrary
$(MODDIR_SRC_PLUGINS)/game_morpion.obj : $(MODDIR_SRC_PLUGINS)/game_morpion.c
	@echo "Build specific game_morpion in src_plugin : $@"
	$(CC) $(STD) /DwithLIB /Dshared_EXPORTS $(CFLAGS) /c $< /Fo"$(dir $@)"

#game_morpion.dll : $(MODDIR_SRC_PLUGINS)/game_morpion.obj $(MODDIR_JOUEUR)/score_game.obj
game_morpion.dll : $(MODDIR_SRC_PLUGINS)/game_morpion.obj $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CLOGGER)
	$(LINK) $(LFLAGS) /DLL $^ /NOIMPLIB /OUT:game_morpion.dll
#	cp $@ .\plugins, do not mage to copy Copy-Item

clean ::
	@echo "Clean module src_plugin"
	del $(MODDIR_SRC_PLUGINS)\*.obj
	del game_morpion.dll, game_morpion.ilk,game_morpion.exp, game_morpion.pdb

