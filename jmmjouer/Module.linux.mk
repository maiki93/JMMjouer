
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
include jmmjouer/game_loader/Module.linux.mk

# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer/games/Module.linux.mk

###### Main executables

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
# ldd JMMjouer no link to game_pendu (not called to start_game_pendu?)
JMMjouer: $(OBJS_JMMJOUER) main.o $(LIB_STATIC) $(LIB_DYNAMIC) libgame_pendu
	@echo "Build $(EXE): all dependencies $^"
	$(CC) -o $@ $(OBJS_JMMJOUER) main.o -L. -lgame_pendu -lgame_loader -lrecord -ljoueur -lccontainer -lclogger

# same, ldd give no game_pendu
JMMjouer_nolib : main.o $(OBJS_JMMJOUER) $(OBJS_ALL_STATIC)
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
	$(CC) $(STD) $(CFLAGS) -o $@ $^ -L. -lgame_pendu

# test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)/Module_test.linux.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)
	rm -f JMMjouer JMMjouer_nolib
