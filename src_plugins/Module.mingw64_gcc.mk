
MODDIR_SRC_PLUGINS := src_plugins

OBJS_SRC_PLUGINS := $(MODDIR_SRC_PLUGINS)/game_morpion.o

$(info == SRC_PLUGINS ==)
$(info $$OBJS_SRC_PLUGINS is [$(OBJS_SRC_PLUGINS)] )

# but victory is enought for windows
# libclogger not need on linux (option or depends on dynamic/static)
libgame_morpion.dll : $(OBJS_SRC_PLUGINS) libjoueur libclogger
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_SRC_PLUGINS) -L . -ljoueur -lclogger

# only shared library
libgame_morpion : libgame_morpion.dll

clean ::
	@echo "Clean module src plugin"
	rm -f $(OBJS_SRC_PLUGINS)
	rm -f libgame_morpion.dll
