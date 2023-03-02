
MODDIR_SRC_PLUGIN := src_plugin

OBJS_SRC_PLUGIN_MORP := $(MODDIR_SRC_PLUGIN)/game_morpion.o

$(info == SRC_PLUGIN ==)
$(info $$OBJS_SRC_PLUGIN_MORP is [$(OBJS_SRC_PLUGIN_MORP)] )

libgame_morpion.so : $(OBJS_SRC_PLUGIN_MORP) libjoueur
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_SRC_PLUGIN_MORP) -L . -ljoueur

# utils clear stdin : not included, found in core jmmjouer...
#libgame_morpion.so : $(OBJS_SRC_PLUGIN_MORP) joueur/victory.o
#	$(CC) -shared $(CFLAGS) -o $@ $^

# only shared library
libgame_morpion : libgame_morpion.so

clean ::
	@echo "Clean module src plugin"
	rm -f $(OBJS_SRC_PLUGIN_MORP)
	rm -f libgame_morpion.so


