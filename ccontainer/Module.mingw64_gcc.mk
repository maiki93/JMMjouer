
MODDIR_CCONTAINER := ccontainer

SRCS_CCONTAINER := $(wildcard $(MODDIR_CCONTAINER)/*.c)
OBJS_CCONTAINER := $(patsubst %.c, %.o, $(SRCS_CCONTAINER))
# update global variable
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

# not so clear, use *lib or *a for windows ?
# try keeping lib (and see if -lccontainer is working), seems working
# https://stackoverflow.com/questions/43311621/c-correct-way-to-statically-dynamically-link-with-mingw-w64
libccontainer.a : $(OBJS_CCONTAINER)
	ar rcs $@ $^

ifneq ($(findstring libccontainer,$(LIB_STATIC)),)
IMPORT_LIB_CCONTAINER = ccontainer
libccontainer : libccontainer.a
else
IMPORT_LIB_CCONTAINER = ccontainer_dll
libccontainer : libccontainer.dll
endif

# include description for each module
include $(MODDIR_CCONTAINER)/tests/Module_test.mingw64_gcc.mk

clean ::
	@echo "Clean module ccontainer"
	rm -f $(MODDIR_CCONTAINER)/*.o
	rm -f libccontainer.a libccontainer.dll libccontainer_dll.a
