#
# For windows msvc, usual module also
# tests defined in the same file
MODDIR_CLOGGER := clogger

SRCS_CLOGGER := $(wildcard $(MODDIR_CLOGGER)/*.c)
OBJS_CLOGGER := $(patsubst %.c, %.obj, $(SRCS_CLOGGER))

OBJS_ALL_STATIC += $(OBJS_CLOGGER)

$(info)
$(info == CLOGGER : $(MODDIR_CLOGGER) ==)
$(info $$SRCS_CLOGGER is [$(SRCS_CLOGGER)] )
$(info $$OBJS_CLOGGER is [$(OBJS_CLOGGER)] )

$(OBJS_CLOGGER): %.obj: %.c
	@echo "Build specific rule in clogger : $@"
	$(CC) $(SHARED_EXPORT_CLOGGER) $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

libclogger.dll : $(OBJS_CLOGGER)
	@echo "Create shared library -- clogger"
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:libclogger_dll.lib /OUT:libclogger.dll

# allow to use *_dll.lib in dependencies
libclogger_dll.lib : libclogger.dll

libclogger.lib : $(OBJS_CLOGGER)
	@echo "Create static library -- clogger"
	$(LINK) /LIB $(LFLAGS) $^ /OUT:libclogger.lib

## to use for dependencies call
ifneq ($(findstring libclogger,$(LIB_STATIC)),)
undefine SHARED_EXPORT_CLOGGER
IMPORT_LIB_CLOGGER = libclogger.lib
libclogger : libclogger.lib
else ifneq ($(findstring libclogger,$(LIB_DYNAMIC)),)
SHARED_EXPORT_CLOGGER = /DwithLIB /Dshared_EXPORTS
IMPORT_LIB_CLOGGER = libclogger_dll.lib
libclogger : libclogger.dll
else
undefine SHARED_EXPORT_CLOGGER
IMPORT_LIB_CLOGGER = $(OBJS_CLOGGER)
endif
# Could be the list of object files ?
$(info IMPORT_LIB_CLOGGER: $(IMPORT_LIB_CLOGGER) )
$(info SHARED_EXPORT_CLOGGER: $(SHARED_EXPORT_CLOGGER) )

#### TU
OBJS_ALL_TESTS += $(MODDIR_CLOGGER)/tests/test_clogger.obj

unit_test :: test_clogger

 # solve problem (when shared library and include only *.obj) with /NOIMPLIB /NOEXP, whatever the compilation
# or always use the library (shared or static), keep this
#test_clogger: $(MODDIR_CLOGGER)/tests/test_clogger.obj $(OBJS_CLOGGER)
test_clogger: $(MODDIR_CLOGGER)/tests/test_clogger.obj $(IMPORT_LIB_CLOGGER)
	@echo "Building test_clogger @ :    $@"  # target name
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib
	
clean ::
	@echo "Clean module clogger"
	del $(MODDIR_CLOGGER)\*.obj
	del libclogger.dll, libclogger_dll.lib, libclogger.lib, libclogger_dll.exp
	del $(MODDIR_CLOGGER)\tests\test_clogger.obj
	del test_clogger.exe
