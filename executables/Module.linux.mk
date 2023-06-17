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

#OBJS_ALL_STATIC += $(OBJS_EXEC)

$(info == EXECUTABLES : $(MODDIR_EXEC) ==)
$(info $$OBJS_EXEC is [$(OBJS_EXEC)] )


NO_LIB ?= 0
# equivalent is
#ifeq ($(origin NO_LIB), undefined)
# NO_LIB = 0
#endif
ifeq ($(NO_LIB),0)
	EXE:= JMMjouer
else
	EXE:= JMMjouer_nolib
endif

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
# ldd JMMjouer no link to game_pendu (not called to start_game_pendu?)
JMMjouer: $(OBJS_EXEC) $(OBJS_JMMJOUER) $(LIB_STATIC) $(LIB_DYNAMIC) libgame_pendu
	@echo "Build $(EXE): all dependencies $^"
	$(CC) -o $@ $(OBJS_EXEC) $(OBJS_JMMJOUER) -L. -lrecord -ljoueur -lgame_pendu -lccontainer -lclogger

# same, ldd give no game_pendu $(OBJS_JMMJOUER)
JMMjouer_nolib : $(OBJS_EXEC) $(OBJS_ALL_STATIC) libgame_pendu
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
	$(CC) $(STD) $(CFLAGS) -o $@ $(OBJS_EXEC) $(OBJS_ALL_STATIC) -L. -lgame_pendu

clean::
	@echo "Clean in executables"
	rm -f $(OBJS_EXEC)
	rm -f JMMjouer JMMjouer_nolib