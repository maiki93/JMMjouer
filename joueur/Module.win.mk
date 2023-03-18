
MODDIR_JOUEUR = joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR = $(patsubst %.c, %.obj, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

# defined here, used in tests/Module.X.mk
MODDIR_JOUEUR_TESTS = $(MODDIR_JOUEUR)\tests

$(info == JOUEUR : $(MODDIR_JOUEUR) ==)
$(info $$SRCS_JOUEUR is [$(SRCS_JOUEUR)] )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

# needed with msvc to deal with shared / static library
$(OBJS_JOUEUR): %.obj: %.c
	@echo "Build file specific rule in joueur : $@"
#	$(CC) $(WITH_LIB) /Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"
	$(CC) $(STD) $(SHARED_EXPORT_JOUEUR) $(CFLAGS) /c $< /Fo"$(dir $@)"

# provoque creation of shared/static library clogger and ccontainer
libjoueur.dll : $(OBJS_JOUEUR) $(IMPORT_LIB_CLOGGER) $(IMPORT_LIB_CCONTAINER)
	@echo "Create shared library -- libjoueur"
	$(LINK) $(LFLAGS) /DLL $^  /IMPLIB:libjoueur_dll.lib /OUT:libjoueur.dll

libjoueur_dll.lib : libjoueur.dll

libjoueur.lib : $(OBJS_JOUEUR)
	@echo "Create static library -- libjoueur"
	$(LINK) /LIB $(LFLAGS) $^ /OUT:$@

## to use for dependencies call
#$(info TEST: $(findstring libjoueur,$(LIB_STATIC)) )
ifneq ($(findstring libjoueur,$(LIB_STATIC)),)
undefine SHARED_EXPORT_JOUEUR
IMPORT_LIB_JOUEUR = libjoueur.lib
libjoueur : libjoueur.lib
else ifneq ($(findstring libjoueur,$(LIB_DYNAMIC)),)
SHARED_EXPORT_JOUEUR = /DwithLIB /Dshared_EXPORTS
IMPORT_LIB_JOUEUR = libjoueur_dll.lib
libjoueur : libjoueur.dll
else
undefine SHARED_EXPORT_JOUEUR
#undefine IMPORT_LIB_JOUEUR
IMPORT_LIB_JOUEUR = $(OBJS_JOUEUR)
endif

# include description for each module
#include $(patsubst %,%/Module_test.win.mk,$(MODDIR_CCONTAINER_TESTS))
include $(MODDIR_JOUEUR)/tests/Module_test.win.mk

clean ::
	@echo "Clean module joueur"
	del $(MODDIR_JOUEUR)\*.obj
	del libjoueur.dll, libjoueur_dll.lib, libjoueur.lib, libjoueur_dll.exp
