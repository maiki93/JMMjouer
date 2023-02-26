
MODDIR_GAME_LOADER := game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER := $(patsubst %.c, %.o, $(SRCS_GAME_LOADER))

# update global variable
OBJS_ALL_STATIC += $(OBJS_GAME_LOADER)

$(info == GAME_LOADER ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

libgame_loader.so : $(OBJS_GAME_LOADER)
	@echo "Create shared library -- game_loader"
#	$(CC) -shared -o $@ $(OBJS_GAME_LOADER) -L . -lclogger -lccontainer
	$(CC) -shared -o $@ $(OBJS_GAME_LOADER)

libgame_loader.a : $(OBJS_GAME_LOADER)
	@echo "Create static library -- game_loader"
	ar rcs $@ $^

ifneq ($(findstring libgame_loader,$(LIB_STATIC)),)
libgame_loader : libgame_loader.a
else
libgame_loader : libgame_loader.so
endif

clean ::
	@echo "Clean module record"
	rm -f $(MODDIR_GAME_LOADER)/*.o
	rm -f libgame_loader.so libgame_loader.a
