#
# Main executables, access to all librairies from an idependent module
#
# v1 : simple, only use for compilation of main
## TU not really, but tests integrations(?) / perf (good place) 

# v2 : a libjmmjouer_core.dll ? certainly when more complex

# Access to a library core libjmmjouer_core
# or include all objects from jmmjouer

MODDIR_EXEC := executables

# getopt for windows
OBJS_EXEC = $(MODDIR_EXEC)/main.o

$(info == EXECUTABLES : $(MODDIR_EXEC) ==)
$(info $$OBJS_EXEC is [$(OBJS_EXEC)] )

NO_LIB ?= 0
ifeq ($(NO_LIB),0)
	EXE:= JMMjouer
else
	EXE:= JMMjouer_nolib
endif

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
#JMMjouer: $(OBJS_EXE) main.o libgame_pendu.so
#JMMjouer: $(OBJS_EXE) main.o libclogger.so
JMMjouer: $(OBJS_EXEC) $(OBJS_JMMJOUER) $(LIB_STATIC) $(LIB_DYNAMIC) libgame_pendu.dll
	@echo "Build $(EXE): all dependencies $^"
#	$(CC) -o $@ $(OBJS_JMMJOUER) main.o -L .  -lgame_loader -lrecord -ljoueur -lccontainer -lclogger
	$(CC) -o $@ $(OBJS_EXEC) $(OBJS_JMMJOUER) -L.  -l$(IMPORT_LIB_GAME_LOADER) -l$(IMPORT_LIB_RECORD) -l$(IMPORT_LIB_JOUEUR) -l$(IMPORT_LIB_CCONTAINER) -l$(IMPORT_LIB_CLOGGER) -lgame_pendu_dll

JMMjouer_nolib : $(OBJS_EXEC) $(OBJS_ALL_STATIC) libclogger libgame_pendu.dll
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
	$(CC) $(STD) $(CFLAGS) -o $@ $(OBJS_EXEC) $(OBJS_ALL_STATIC) -L. -l$(IMPORT_LIB_CLOGGER) -lgame_pendu_dll

clean::
	@echo "Clean in executables"
	rm -f $(OBJS_EXEC)
	rm -f JMMjouer.exe JMMjouer_nolib.exe