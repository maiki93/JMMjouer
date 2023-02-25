
MODDIR_JOUEUR = joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR = $(patsubst %.c, %.obj, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

$(info == JOUEUR ==)
$(info $$SRCS_JOUEUR is [$(SRCS_JOUEUR)] )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

# needed ONLY for passing -Dshared_export now
$(OBJS_JOUEUR): %.obj: %.c
	@echo "Build file specific rule in joueur : $@"
	$(CC) $(WITH_LIB) /Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"


libjoueur.dll : $(OBJS_JOUEUR) libclogger_dll.lib libccontainer_dll.lib
	@echo "Create shared library -- joueur"
	$(LINK) $(LFLAGS) /DLL $^  /IMPLIB:libjoueur_dll.lib /OUT:libjoueur.dll

libjoueur_dll.lib : libjoueur.dll

clean ::
	@echo "Clean module joueur"
	del $(MODDIR_JOUEUR)\*.obj
	del libjoueur.dll, libjoueur_dll.lib, libjoueur_dll.exp
