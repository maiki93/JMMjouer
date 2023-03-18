
MODDIR_RECORD = record

SRCS_RECORD := $(wildcard $(MODDIR_RECORD)/*.c)
OBJS_RECORD = $(patsubst %.c, %.obj, $(SRCS_RECORD))

# update global variable
OBJS_ALL_STATIC += $(OBJS_RECORD)

MODDIR_RECORD_TESTS = record\tests

$(info == RECORD : $(MODDIR_RECORD) ==)
$(info $$SRCS_RECORD is [$(SRCS_RECORD)] )
$(info $$OBJS_RECORD is [$(OBJS_RECORD)] )

# needed ONLY for passing -Dshared_export now
$(OBJS_RECORD): %.obj: %.c
	@echo "Build specific rule in record : $@"
	$(CC) $(STD) $(SHARED_EXPORT_RECORD) $(CFLAGS) /c $< /Fo"$(dir $@)"

# use DEF file to force export of the API
librecord.dll : $(OBJS_RECORD) $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Create shared library -- record"
	$(LINK) $(LFLAGS) /DLL $^ /DEF:record/record_dll.def /IMPLIB:librecord_dll.lib /OUT:librecord.dll

librecord_dll.lib : librecord.dll

# Static library
librecord.lib : $(OBJS_RECORD)
	@echo "Create static library -- librecord"
	$(LINK) /LIB $(LFLAGS) $^ /OUT:$@

## to use for dependencies call
ifneq ($(findstring librecord,$(LIB_STATIC)),)
undefine SHARED_EXPORT_RECORD
IMPORT_LIB_RECORD = librecord.lib
librecord : librecord.lib
else ifneq ($(findstring librecord,$(LIB_DYNAMIC)),)
SHARED_EXPORT_RECORD = /DwithLIB /Dshared_EXPORTS
IMPORT_LIB_RECORD = librecord_dll.lib
librecord : librecord.dll
else
undefine SHARED_EXPORT_RECORD
IMPORT_LIB_RECORD = $(OBJS_RECORD)
endif
$(info IMPORT_LIB_RECORD: $(IMPORT_LIB_RECORD) )
$(info SHARED_EXPORT_RECORD: $(SHARED_EXPORT_RECORD) )

include $(MODDIR_RECORD_TESTS)\Module_test.win.mk

clean ::
	@echo "Clean module record"
	del $(MODDIR_RECORD)\*.obj
	del librecord.dll, librecord_dll.lib, librecord.lib, librecord_dll.exp
