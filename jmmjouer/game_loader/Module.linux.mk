
# path relative to root, more flexible
MODDIR_GAME_LOADER := jmmjouer/game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER := $(patsubst %.c, %.o, $(SRCS_GAME_LOADER))

$(info == GAME_LOADER : $(MODDIR_GAME_LOADER) ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

# specific cannot use -z defs because start_game_pendu is not yet known. ok for *.a, must be added only later at executable
# use of extern would allow -z defs ?? certainly to try.
#LFLAGS_GL = --warn-common -shared --no-allow-shlib-undefined -lc

# with utils added pass the undefined constraint
#libgame_loader.so : $(OBJS_GAME_LOADER) $(OBJS_GAMES) jmmjouer/utils.o jmmjouer/utils_file.o libjoueur libccontainer libclogger
#	@echo "Create shared library -- game_loader"
#	$(LINK) $(LFLAGS_GL) -o $@ $(OBJS_GAME_LOADER) $(OBJS_GAMES) jmmjouer/utils.o jmmjouer/utils_file.o -L. -ljoueur -lccontainer -lclogger

#libgame_loader.a : $(OBJS_GAME_LOADER) $(OBJS_GAMES)
#	@echo "Create static library -- game_loader"
#	ar rcs $@ $^

#ifneq ($(findstring libgame_loader,$(LIB_STATIC)),)
#libgame_loader : libgame_loader.a
#else
#libgame_loader : libgame_loader.so
#endif

include $(MODDIR_GAME_LOADER)/tests/Module_test.linux.mk

clean ::
	@echo "Clean module game_loader"
#	rm -f $(MODDIR_GAME_LOADER)/*.o
#	rm -f libgame_loader.so libgame_loader.a
