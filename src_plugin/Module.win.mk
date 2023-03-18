
MODDIR_SRC_PLUGIN := src_plugin

OBJS_SRC_PLUGIN_MORP := $(MODDIR_SRC_PLUGIN)/game_morpion.obj

$(info == SRC_PLUGIN : $(MODDIR_SRC_PLUGIN) ==)
$(info $$OBJS_SRC_PLUGIN_MORP is [$(OBJS_SRC_PLUGIN_MORP)] )

# independent of other make config, always shared dlibrary
$(MODDIR_SRC_PLUGIN)/game_morpion.obj : $(MODDIR_SRC_PLUGIN)/game_morpion.obj
	@echo "Build specific game_morpion in src_plugin : $@"
	$(CC) $(STD) /DwithLIB /Dshared_EXPORTS $(CFLAGS) /c $< /Fo"$(dir $@)"

game_morpion.dll : $(MODDIR_SRC_PLUGIN)/game_morpion.obj $(MODDIR_JOUEUR)/victory.obj
#	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:game_pendu_dll.lib /OUT:game_pendu.dll
	$(LINK) $(LFLAGS) /DLL $^ /NOIMPLIB /OUT:game_morpion.dll

clean ::
	@echo "Clean module src_plugin"
	del $(MODDIR_SRC_PLUGIN)\*.obj
#	del game_morpion.dll 

