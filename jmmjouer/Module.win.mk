
MODDIR_JMMJOUER := jmmjouer

OBJS_JMMJOUER = $(MODDIR_JMMJOUER)/utils.obj $(MODDIR_JMMJOUER)/utils_file.obj \
				$(MODDIR_JMMJOUER)/arcade.obj

OBJS_ALL_STATIC += $(OBJS_JMMJOUER)

MODDIR_JMMJOUER_TESTS = $(MODDIR_JMMJOUER)\tests

$(info == CORE JMMJOUER ==)
#$(info $$SRCS_JMMJOUER is [$(SRCS_JMMJOUER)] )
$(info $$OBJS_JMMJOUER is [$(OBJS_JMMJOUER)] )

# subdirectory in jmmjouer core
# update same global list : OBJS_ALL_STATIC, OBJS_ALL_TESTS
# include the games BEFORE : game_pendu compiled as shared library and linked at compilation
include jmmjouer\games\Module.win.mk
include jmmjouer/game_loader/Module.win.mk

# In this module, we consume all the libraries, overidde SHARED_EXPORTS with import
$(OBJS_JMMJOUER): %.obj: %.c
	@echo "Build file specific rule in jmmjouer : $@"
	$(CC) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"


# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
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


# test core, integration tests
include $(MODDIR_JMMJOUER_TESTS)\Module_test.win.mk

clean::
	@echo "Clean in jmmjouer core directory"
	del $(MODDIR_JMMJOUER)\*.obj

