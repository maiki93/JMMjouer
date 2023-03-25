
MODDIR_JMMJOUER := jmmjouer

# Use explicit OBJS, more control
# do not include main.o
OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.obj $(MODDIR_JMMJOUER)/utils_file.obj \
				$(MODDIR_JMMJOUER)/arcade.obj $(MODDIR_JMMJOUER)/getopt.obj

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)\tests

$(info == CORE JMMJOUER ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
include jmmjouer/game_loader/Module.win.mk

# moved above, should still compile
# include the games : game_pendu compiled as shared library and linked at compilation
include jmmjouer\games\Module.win.mk

# In this module, we consume all the libraries, overidde SHARED_EXPORTS with import

$(OBJS_JMMJOUER): %.obj: %.c
	@echo "Build file specific rule in jmmjouer : $@"
	$(CC) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

# with this, only game_pendu is still exported, without many more
# getopt.obj C4131, C4100, but makefile warning overidding => takes the last one
jmmjouer/getopt.obj : jmmjouer/getopt.C
	@echo "Build getopt.obj specific rule in jmmjouer : $@"
	$(CC) $(STD) $(CFLAGS) /wd4131 /wd4100 /c $< /Fo"$(dir $@)"

# executable should be apart in an executable indeed
main.obj : main.c
	@echo "Build main : $@"
#	$(CC) $(STD) $(CFLAGS) /DwithLIB /c $< /Fo"$(dir $@)"
	$(CC) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
#game_pendu.dll librecord_dll.lib 
# /CGTHREADS
# /GENPROFILE, /FASTGENPROFILE, /LTCG, /PGD, /PROFILE, /USEPROFILE
# /SUBSYSTEM [:POSIX and other..]
# //HEAP : size of heap (for tests?)
# /FORCE[:MULTIPLE|:UNRESOLVED] : force even if undefined symbol or multiple ones
# /EXPORT : 4 ways
# https://learn.microsoft.com/en-us/cpp/build/reference/export-exports-a-function?view=msvc-170
# /INCLUDE : include in symbol table
# /MAP filename, /MAPINFO
# /OPT:{REF | NOREF} : can supprress unused references


# now compile (still JMMjouer.lib produced), but with static record library
# dumpbin.exe /EXPORTS .\JMMjouer.lib => game_pendu.lib / nm -gC also
JMMjouer : $(OBJS_JMMJOUER) main.obj game_pendu_dll.lib $(IMPORT_LIB_GAME_LOADER) $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_RECORD) \
			$(IMPORT_LIB_GAME_LOADER) $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
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
JMMjouer_nolib : main.obj $(OBJS_EXE) $(OBJS_ALL_STATIC)
	@echo "Build JMMjouer_nolib ^: all dependencies $^"
	@echo "Build JMMjouer_nolib <: all dependencies $<"
# order is important !! in fact not...
#	$(LINK) $(LFLAGS) $^ /OUT:JMMjouer_static.exe
#	$(LINK) $(LFLAGS) /OUT:JMMjouer_static.exe $^
	$(LINK) $(LFLAGS) /OUT:JMMjouer_nolib.exe $^
#	$(LINK) $(LFLAGS) /OUT:JMMjouer_static.exe $^ /LIBPATH:. game_pendu.lib

# test core, integration tests
#include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_JMMJOUER_TESTS))
include $(MODDIR_JMMJOUER_TESTS)\Module_test.win.mk

clean::
	@echo "Clean in jmmjouer core directory"
	del $(MODDIR_JMMJOUER)\*.obj, main.obj
	del JMMjouer.exe JMMjouer_nolib.exe
