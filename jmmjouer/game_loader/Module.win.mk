
# Just replace JOUEUR / RECORD && joueur / record
MODDIR_GAME_LOADER = jmmjouer\game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER = $(patsubst %.c, %.obj, $(SRCS_GAME_LOADER))

# update global variable
OBJS_ALL_STATIC += $(OBJS_GAME_LOADER)

$(info == GAME_LOADER : $(MODDIR_GAME_LOADER) ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

# needed ONLY for passing -Dshared_export now, module specific
$(OBJS_RECORD): %.obj: %.c
	@echo "Build specific rule in game_loader : $@"
	$(CC) $(STD) $(SHARED_EXPORT_GAME_LOADER) $(CFLAGS) /c $< /Fo"$(dir $@)"

# build dll, but no export so cannot finalize compile (and multiple def...)
# in fact game loader could be in core, the plugin manager should be apart / reusable
ADDITIONAL_DEP = $(MODDIR_JMMJOUER)/utils.obj $(MODDIR_JMMJOUER)/utils_file.obj

# try shared dll with msvc, missing some extern from clist_cstring, BUT LASO utils, utils_file
#     same as Mingw 
# libclogger_dll.lib included twice ??
libgame_loader.dll : $(OBJS_GAME_LOADER) $(ADDITIONAL_DEP) $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Create shared library -- game_loader"
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:libgame_loader_dll.lib /OUT:libgame_loader.dll

libgame_loader_dll.lib : libgame_loader.dll

# Static library, compile without -Dshared_EXPORTS
libgame_loader.lib : $(OBJS_GAME_LOADER)
	@echo "Create static library -- game_loader"
#	link.exe -lib $^ /OUT:libgame_loader.lib
	$(LINK) /lib $(LFLAGS) $^ /OUT:libgame_loader.lib

## to use for dependencies call
ifneq ($(findstring libgame_loader,$(LIB_STATIC)),)
undefine SHARED_EXPORT_GAME_LOADER
IMPORT_LIB_GAME_LOADER = libgame_loader.lib
libgame_loader : libgame_loader.lib
else ifneq ($(findstring libgame_loader,$(LIB_DYNAMIC)),)
SHARED_EXPORT_GAME_LOADER = /DwithLIB /Dshared_EXPORTS
IMPORT_LIB_GAME_LOADER = libgame_loader_dll.lib
libgame_loader : libgame_loader.dll
else
undefine SHARED_EXPORT_GAME_LOADER
IMPORT_LIB_GAME_LOADER = $(OBJS_GAME_LOADER) # $ADD_REP ?
endif
$(info IMPORT_LIB_GAME_LOADER: $(IMPORT_LIB_GAME_LOADER) )

include $(MODDIR_GAME_LOADER)\tests\Module_test.win.mk

clean ::
	@echo "Clean module game_loader"
	del $(MODDIR_GAME_LOADER)\*.obj
	del libgame_loader.dll, libgame_loader_dll.lib, libgame_loader.lib, libgame_loader_dll.exp
