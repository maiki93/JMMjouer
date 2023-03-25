MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
# do not include main.o
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.o $(MODDIR_JMMJOUER)/utils_file.o \
				$(MODDIR_JMMJOUER)/arcade.o

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)/tests

$(info == CORE JMMJOUER : $(MODDIR_JMMOUER) ==)
$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
include jmmjouer/game_loader/Module.mingw64_gcc.mk

# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer/games/Module.mingw64_gcc.mk

###### Main executables

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
#JMMjouer: $(OBJS_EXE) main.o libgame_pendu.so
#JMMjouer: $(OBJS_EXE) main.o libclogger.so
JMMjouer: $(OBJS_JMMJOUER) main.o libgame_pendu.dll $(LIB_STATIC) $(LIB_DYNAMIC)
	@echo "Build $(EXE): all dependencies $^"
#	$(CC) -o $@ $(OBJS_JMMJOUER) main.o -L .  -lgame_loader -lrecord -ljoueur -lccontainer -lclogger
	$(CC) -o $@ $(OBJS_JMMJOUER) main.o -L .  -lgame_pendu_dll -l$(IMPORT_LIB_GAME_LOADER) -l$(IMPORT_LIB_RECORD) -l$(IMPORT_LIB_JOUEUR) -l$(IMPORT_LIB_CCONTAINER) -l$(IMPORT_LIB_CLOGGER)

JMMjouer_nolib : main.o $(OBJS_JMMJOUER) $(OBJS_ALL_STATIC) #libclogger
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
# miss logger because module apart
#	$(CC) $(STD) $(CFLAGS) -o $@ $^
	$(CC) $(STD) $(CFLAGS) -o $@ $^ -L. -l$(IMPORT_LIB_CLOGGER)

# test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)/Module_test.mingw64_gcc.mk

clean::
	@echo "Clean in jmmjouer core directory"
	rm -f $(OBJS_JMMJOUER)
	rm -f JMMjouer.exe JMMjouer_nolib.exe
