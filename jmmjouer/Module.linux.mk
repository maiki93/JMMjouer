
MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
# do not include main.o
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.o $(MODDIR_JMMJOUER)/utils_file.o \
				$(MODDIR_JMMJOUER)/arcade.o

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)/tests

$(info == CORE JMMJOUER ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
include jmmjouer/game_loader/Module.linux.mk

###### Main executables

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
#JMMjouer: $(OBJS_EXE) main.o libgame_pendu.so
#JMMjouer: $(OBJS_EXE) main.o libclogger.so
JMMjouer: $(OBJS_JMMJOUER) main.o $(LIB_STATIC) $(LIB_DYNAMIC)
	@echo "Build $(EXE): all dependencies $^"
	$(CC) -o $@ $(OBJS_JMMJOUER) main.o -L .  -lgame_loader -lrecord -ljoueur -lccontainer -lclogger

JMMjouer_nolib : main.o $(OBJS_JMMJOUER) $(OBJS_ALL_STATIC)
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
	$(CC) $(STD) $(CFLAGS) -o $@ $^

# test core, integration tests
#include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_JMMJOUER_TESTS))
include $(MODDIR_JMMJOUER_TESTS)/Module_test.linux.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)
	rm -f JMMjouer JMMjouer_nolib
