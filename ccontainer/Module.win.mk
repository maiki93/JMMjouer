
MODDIR_CCONTAINER := ccontainer

SRCS_CCONTAINER := $(wildcard $(MODDIR_CCONTAINER)/*.c)
OBJS_CCONTAINER := $(patsubst %.c, %.obj, $(SRCS_CCONTAINER))

OBJS_ALL_STATIC += $(OBJS_CCONTAINER)

# defined here, used in tests/Module.X.mk
MODDIR_CCONTAINER_TESTS = $(MODDIR_CCONTAINER)/tests

$(info == CCONTAINER : $(MODDIR_CCONTAINER) ==)
$(info $$SRCS_CCONTAINER is [$(SRCS_CCONTAINER)] )
$(info $$OBJS_CCONTAINER is [$(OBJS_CCONTAINER)] )
$(info $$LIB_STATIC is [$(LIB_STATIC)])
$(info $$LIB_DYNAMIC is [$(LIB_DYNAMIC)])

# needed ONLY for passing -Dshared_export now
$(OBJS_CCONTAINER): %.obj: %.c
	@echo "Build specific rule in ccontainer : $@"
	$(CC) $(SHARED_EXPORT_CCONTAINER) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"


libccontainer.dll : $(OBJS_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Create shared library -- ccontainer"
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:libccontainer_dll.lib /OUT:libccontainer.dll

# allow to use *_dll.lib in dependencies
libccontainer_dll.lib : libccontainer.dll

libccontainer.lib : $(OBJS_CCONTAINER)
	@echo "Create static library -- ccontainer"
	$(LINK) /LIB $(LFLAGS) $^ /OUT:$@

## to use for dependencies call
$(info TEST: $(findstring libccontainer,$(LIB_STATIC)) )
$(info TEST: $(findstring libccontainer,$(LIB_DYNAMIC)) )
# return the string if found, if different de empty execute
# case static library
ifneq ($(findstring libccontainer,$(LIB_STATIC)),)
undefine SHARED_EXPORT_CCONTAINER
IMPORT_LIB_CCONTAINER = libccontainer.lib
libccontainer : libccontainer.lib
# case shared library
else ifneq ($(findstring libccontainer,$(LIB_DYNAMIC)),)
SHARED_EXPORT_CCONTAINER = /DwithLIB /Dshared_EXPORTS
IMPORT_LIB_CCONTAINER = libccontainer_dll.lib
libccontainer : libccontainer.dll
# case compilation NO_LIB
else
undefine SHARED_EXPORT_CCONTAINER
#undefine IMPORT_LIB_CCONTAINER
IMPORT_LIB_CCONTAINER = $(OBJS_CCONTAINER)
endif
$(info IMPORT_LIB_CCONTAINER: $(IMPORT_LIB_CCONTAINER) )
$(info SHARED_EXPORT_CCONTAINER: $(SHARED_EXPORT_CCONTAINER) )

# include description for each module
include $(patsubst %,%/Module_test.win.mk,$(MODDIR_CCONTAINER_TESTS))
#include $(MODDIR_CONTAINER)/tests/Module_test.mingw64_gcc.mk

clean ::
	@echo "Clean module ccontainer"
	del $(MODDIR_CCONTAINER)\*.obj
	del libccontainer.dll, libccontainer_dll.lib , libccontainer.lib, libccontainer_dll.exp