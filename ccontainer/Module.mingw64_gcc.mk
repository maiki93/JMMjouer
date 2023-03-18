
MODDIR_CCONTAINER := ccontainer


#SRCS_CCONTAINER := $(wildcard $(MODDIR_CCONTAINER)/*.c)
#OBJS_CCONTAINER := $(patsubst %.c, %.o, $(SRCS_CCONTAINER))
OBJS_CCONTAINER = $(MODDIR_CCONTAINER)/clist_generic.o \
				  $(MODDIR_CCONTAINER)/clist_cstring.o

OBJS_ALL_STATIC += $(OBJS_CCONTAINER)

# defined here, used in tests/Module.X.mk
MODDIR_CCONTAINER_TESTS = $(MODDIR_CCONTAINER)/tests

$(info == CCONTAINER : $(MODDIR_CCONTAINER) ==)
$(info $$SRCS_CCONTAINER is [$(SRCS_CCONTAINER)] )
$(info $$OBJS_CCONTAINER is [$(OBJS_CCONTAINER)] )

# https://www.transmissionzero.co.uk/computing/building-dlls-with-mingw/
# advice to use libX.a
# -s -shared add_basic.c -Wl,--subsystem,windows
# --export-all-symbols : done or not ?
# compile with : must include evrything -lccontainer and -lmsvcrt (whatever defined option)
# ld -shared --no-undefined --no-allow-shlib-undefined ccontainer/clist_generic.o ccontainer/clist_cstring.o -o libccontainer.dll --out-implib libccontainer_dll.a -L. -lclogger -lmsvcrt
libccontainer.dll : $(OBJS_CCONTAINER) #libclogger
	@echo "Create shared library -- ccontainer"
#	$(LINK) $(LFLAGS) -o $@ $(OBJS_CCONTAINER) -L . -lclogger
#	$(CC) -shared -o $@ $(OBJS_CCONTAINER) -L . -lclogger
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_CCONTAINER) -Wl,--out-implib,libccontainer_dll.a -L. -lclogger

libccontainer.lib : $(OBJS_CCONTAINER)
	ar rcs $@ $^

ifneq ($(findstring libccontainer,$(LIB_STATIC)),)
libccontainer : libccontainer.lib
else
libccontainer : libccontainer.dll
endif

# include description for each module
include $(patsubst %,%/Module_test.mingw64_gcc.mk,$(MODDIR_CCONTAINER_TESTS))
#include $(MODDIR_CONTAINER)/tests/Module.mingw64_gcc.mk

clean ::
	@echo "Clean module ccontainer"
	rm -f $(MODDIR_CCONTAINER)/*.o
	rm -f libccontainer.lib libccontainer.dll libccontainer_dll.a
