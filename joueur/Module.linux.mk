
MODDIR_JOUEUR = joueur

SRCS_JOUEUR := $(wildcard $(MODDIR_JOUEUR)/*.c)
OBJS_JOUEUR := $(patsubst %.c, %.o, $(SRCS_JOUEUR))

# update global variable
OBJS_ALL_STATIC += $(OBJS_JOUEUR)

# define tests file
MODDIR_JOUEUR_TESTS = $(MODDIR_JOUEUR)/tests

$(info == JOUEUR ==)
$(info $$SRCS_JOUEUR is [$(SRCS_JOUEUR)] )
$(info $$OBJS_JOUEUR is [$(OBJS_JOUEUR)] )

# all dependencies are coming statically linked (need clogger and ccontainer static)
libjoueur.so : $(OBJS_JOUEUR) libclogger libccontainer
	@echo "Create shared library -- joueur"
#	$(CC) -shared -o $@ $(OBJS_JOUEUR) -L . -lclogger -lccontainer
#	$(LINK) $(LFLAGS) -o $@ $(OBJS_JOUEUR) -L. -lclogger -lccontainer
	$(LINK) $(LFLAGS) -Bstatic -o $@ $(OBJS_JOUEUR) -L. -lclogger -lccontainer

libjoueur.a : $(OBJS_JOUEUR)
	@echo "Create static library -- joueur"
	ar rcs $@ $(OBJS_JOUEUR)

ifneq ($(findstring libjoueur,$(LIB_STATIC)),)
libjoueur : libjoueur.a
else
libjoueur : libjoueur.so
endif

#include $(patsubst %,%/Module.linux.mk,$(MODDIR_JOUEUR)/tests))
include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_JOUEUR_TESTS))

clean ::
	@echo "Clean module joueur"
	rm -f $(OBJS_JOUEUR)
	rm -f libjoueur.so libjoueur.a
