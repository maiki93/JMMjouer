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
OBJS_EXEC = $(MODDIR_EXEC)/main.obj $(MODDIR_EXEC)/getopt.obj

$(info == EXECUTABLES : $(MODDIR_EXEC) ==)
$(info $$OBJS_EXEC is [$(OBJS_EXEC)] )

NO_LIB ?= 0
ifeq ($(NO_LIB),0)
	EXE:= JMMjouer
else
	EXE:= JMMjouer_nolib
endif

# with this, only game_pendu is still exported, without many more
# getopt.obj C4131, C4100, but makefile warning overidding => takes the last one
executables/getopt.obj : executables/getopt.C
	@echo "Build getopt.obj specific rule in jmmjouer : $@"
	$(CC) $(STD) $(CFLAGS) /wd4131 /wd4100 /c $< /Fo"$(dir $@)"

# executable should be apart in an executable indeed
executable/main.obj : executables/main.c
	@echo "Build main : $@"
#	$(CC) $(STD) $(CFLAGS) /DwithLIB /c $< /Fo"$(dir $@)"
	$(CC) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

# now compile (still JMMjouer.lib produced), but with static record library
# dumpbin.exe /EXPORTS .\JMMjouer.lib => game_pendu.lib / nm -gC also
JMMjouer : $(OBJS_EXEC) $(OBJS_JMMJOUER) game_pendu_dll.lib $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_RECORD) \
			$(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Build $(EXE): all dependencies $^"
	$(LINK) $(LFLAGS) $^ /OUT:JMMjouer.exe
#	$(LINK) $(LFLAGS) /OUT:JMMjouer.exe $^ /LIBPATH:. librecord.dll
#	$(LINK) $(LFLAGS) /OUT:JMMjouer.exe $^ librecord.lib
#	$(LINK) $(LFLAGS) /OUT:JMMjouer.exe main.obj $(OBJS_EXE) /LIBPATH:. game_pendu.lib

#JMMjouer: $(OBJS_JMMJOUER) main.obj libgame_loader.lib libjoueur_dll.lib \
#					libccontainer_dll.lib librecord_dll.lib libclogger_dll.lib 
#	@echo "Build $(EXE): all dependencies $^"
#	$(LINK) $(LFLAGS) /OUT:JMMjouer.exe $^

# it produces *lib and *exp files , certianly because of exporting 
JMMjouer_nolib : $(OBJS_EXEC) $(OBJS_ALL_STATIC) game_pendu.dll
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
# order is important !! in fact not...
#	$(LINK) $(LFLAGS) $^ /OUT:JMMjouer_static.exe
#	$(LINK) $(LFLAGS) /OUT:JMMjouer_static.exe $^
#	$(LINK) $(LFLAGS) /OUT:JMMjouer_nolib.exe $^
	$(LINK) $(LFLAGS) /OUT:JMMjouer_nolib.exe $(OBJS_EXEC) $(OBJS_ALL_STATIC) /LIBPATH:. game_pendu_dll.lib

clean::
	@echo "Clean in executables directory"
	del $(MODDIR_EXEC)\*.obj
	del JMMjouer.exe JMMjouer_nolib.exe