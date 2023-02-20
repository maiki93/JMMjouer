
# Just replace JOUEUR / RECORD && joueur / record
MODDIR_GAME_LOADER = game_loader

SRCS_GAME_LOADER := $(wildcard $(MODDIR_GAME_LOADER)/*.c)
OBJS_GAME_LOADER = $(patsubst %.c, %.obj, $(SRCS_GAME_LOADER))

# update global variable
OBJS_ALL_STATIC += $(OBJS_GAME_LOADER)

$(info == GAME_LOADER ==)
$(info $$SRCS_GAME_LOADER is [$(SRCS_GAME_LOADER)] )
$(info $$OBJS_GAME_LOADER is [$(OBJS_GAME_LOADER)] )

# try shared dll with msvc, missing some extern from clist_cstring, BUT LASO utils, utils_file
#     same as Mingw 
# libclogger_dll.lib included twice ??
#libgame_loader.dll : $(OBJS_GAME_LOADER) libjoueur_dll.lib libccontainer_dll.lib libclogger_dll.lib
#	@echo "Create shared library -- game_loader"
#	$(LINK) $(LFLAGS) /DLL $^  /IMPLIB:libgame_loader_dll.lib /OUT:libgame_loader.dll

#libgame_loader_dll.lib : libgame_loader.dll

# Static library, compile without -Dshared_EXPORTS
libgame_loader.lib : $(OBJS_GAME_LOADER)
	@echo "Create static library -- game_loader"
#	link.exe -lib $^ /OUT:libgame_loader.lib
	$(LINK) /lib $(LFLAGS) /OUT:libgame_loader.lib $^ 

clean ::
	@echo "Clean module game_loader"
	del $(MODDIR_GAME_LOADER)\*.obj
	del libgame_loader.dll, libgame_loader_dll.lib, libgame_loader.lib