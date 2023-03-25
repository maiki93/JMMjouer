
# path relative to root, more flexible
MODDIR_GAME_LOADER := jmmjouer/game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER := $(patsubst %.c, %.o, $(SRCS_GAME_LOADER))

# update global variable
OBJS_ALL_STATIC += $(OBJS_GAME_LOADER)

# define tests file
# MODDIR_GAME_LOADER_TESTS = $(MODDIR_GAME_LOADER)/tests

$(info == GAME_LOADER : $(MODDIR_GAME_LOADER) ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

#OBJS_GAME_LOADER += jmmjouer/utils.o jmmjouer/utils_file.o
# with utils added pass the undefined constraint
libgame_loader.so : $(OBJS_GAME_LOADER) jmmjouer/utils.o jmmjouer/utils_file.o libjoueur libccontainer libclogger
	@echo "Create shared library -- game_loader"
	$(LINK) $(LFLAGS) -o $@ $(OBJS_GAME_LOADER) jmmjouer/utils.o jmmjouer/utils_file.o -L. -ljoueur -lccontainer -lclogger

libgame_loader.a : $(OBJS_GAME_LOADER)
	@echo "Create static library -- game_loader"
	ar rcs $@ $^

ifneq ($(findstring libgame_loader,$(LIB_STATIC)),)
libgame_loader : libgame_loader.a
else
libgame_loader : libgame_loader.so
endif

#include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_GAME_LOADER_TESTS))
include $(MODDIR_GAME_LOADER)/tests/Module_test.linux.mk

clean ::
	@echo "Clean module game_loader"
	rm -f $(MODDIR_GAME_LOADER)/*.o
	rm -f libgame_loader.so libgame_loader.a
