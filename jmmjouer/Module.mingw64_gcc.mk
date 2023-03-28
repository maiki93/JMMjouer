
MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.o $(MODDIR_JMMJOUER)/utils_file.o \
				$(MODDIR_JMMJOUER)/arcade.o

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)/tests

$(info == CORE JMMJOUER : $(MODDIR_JMMOUER) ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer/games/Module.mingw64_gcc.mk
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
include jmmjouer/game_loader/Module.mingw64_gcc.mk

# test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)/Module_test.mingw64_gcc.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)

