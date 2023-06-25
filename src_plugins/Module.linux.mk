
MODDIR_SRC_PLUGINS := src_plugins

OBJS_SRC_PLUGINS := $(MODDIR_SRC_PLUGINS)/game_morpion.o

$(info == SRC_PLUGINS ==)
$(info $$OBJS_SRC_PLUGINS is [$(OBJS_SRC_PLUGINS)] )

# here certainly need a rpath
libgame_morpion.so : $(OBJS_SRC_PLUGINS) libjoueur
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_SRC_PLUGINS) -L . -ljoueur

# utils clear stdin : not included, found in core jmmjouer...
#libgame_morpion.so : $(OBJS_SRC_PLUGIN_MORP) joueur/victory.o
#	$(CC) -shared $(CFLAGS) -o $@ $^

# only shared library
libgame_morpion : libgame_morpion.so

clean ::
	@echo "Clean module src plugin"
	rm -f $(OBJS_SRC_PLUGINS)
	rm -f libgame_morpion.so


