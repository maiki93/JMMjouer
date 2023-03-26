
MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
# do not include main.o
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.o $(MODDIR_JMMJOUER)/utils_file.o \
				$(MODDIR_JMMJOUER)/arcade.o

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)/tests

$(info == CORE JMMJOUER : $(MODDIR_JMMOUER) ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer/games/Module.linux.mk
# games before game_loader to build shared library, not nice this dependency. bad organization
include jmmjouer/game_loader/Module.linux.mk

##### test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)/Module_test.linux.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)
	rm -f JMMjouer JMMjouer_nolib
