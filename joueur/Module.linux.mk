
MODDIR_JOUEUR = joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR = $(patsubst %.c, %.o, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

$(info == JOUEUR ==)
$(info $$SRCS_JOUEUR is [$(SRCS_JOUEUR)] )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

#libjoueur.so : $(OBJS_JOUEUR) libclogger.so libccontainer.so
#	@echo "Create shared library -- joueur"
#	$(CC) -shared -o $@ $(OBJS_JOUEUR) -L . -lclogger -lccontainer

libjoueur.so : $(OBJS_JOUEUR) libclogger libccontainer
	@echo "Create shared library -- joueur"
	$(CC) -shared -o $@ $(OBJS_JOUEUR) -L . -lclogger -lccontainer

libjoueur.a : $(OBJS_JOUEUR)
	@echo "Create static library -- joueur"
	ar rcs $@ $(OBJS_JOUEUR)

ifneq ($(findstring libjoueur,$(LIB_STATIC)),)
libjoueur : libjoueur.a
else
libjoueur : libjoueur.so
endif

clean ::
	@echo "Clean module joueur"
	rm -f $(MODDIR_JOUEUR)/*.o
	rm -f libjoueur.so libjoueur.a
