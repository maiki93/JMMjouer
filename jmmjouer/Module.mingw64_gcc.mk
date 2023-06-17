
MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.o $(MODDIR_JMMJOUER)/utils_file.o \
				$(MODDIR_JMMJOUER)/arcade.o

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)/tests

$(info == CORE JMMJOUER : $(MODDIR_JMMOUER) ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer/games/Module.mingw64_gcc.mk
# games before game_loader to build (shared library,) not nice this dependency. bad organization
include jmmjouer/game_loader/Module.mingw64_gcc.mk

# regroup all, included at the same time for the executable
OBJS_JMMJOUER += $(OBJS_GAMES) $(OBJS_GAME_LOADER)
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )
OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

# test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)/Module_test.mingw64_gcc.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)

