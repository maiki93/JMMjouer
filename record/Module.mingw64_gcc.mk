
# Purpose to create a shared libray
#
MODDIR_RECORD := record

SRCS_RECORD := $(wildcard $(MODDIR_RECORD)/*.c)
OBJS_RECORD = $(patsubst %.c, %.o, $(SRCS_RECORD))

# update global variable
OBJS_ALL_STATIC += $(OBJS_RECORD)

# CC, CFLAGS already defined/ shared across Modules
MODDIR_RECORD_TESTS = $(MODDIR_RECORD)/tests

$(info)
$(info == RECORD : $(MODDIR_RECORD) ==)
$(info $$MODDIR_RECORD_TESTS is $(MODDIR_RECORD_TESTS) )

# mingw64, must add all dpendencies !! 
# to check on linux
# dependencies on libjoueur and logger
librecord.dll : $(OBJS_RECORD) libjoueur libclogger libccontainer
	@echo "Create shared library -- record"
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_RECORD) -Wl,--out-implib,librecord_dll.a -L. -l$(IMPORT_LIB_JOUEUR) -l$(IMPORT_LIB_CLOGGER) -l$(IMPORT_LIB_CCONTAINER)

librecord_dll.a : librecord.dll

librecord.a : $(OBJS_RECORD)
	@echo "Create static library -- record"
	ar rcs $@ $^

ifneq ($(findstring librecord,$(LIB_STATIC)),)
IMPORT_LIB_RECORD = record
librecord : librecord.a
else
IMPORT_LIB_RECORD = record_dll
librecord : librecord.dll
endif

# include module of tests
include $(patsubst %,%/Module_test.mingw64_gcc.mk,$(MODDIR_RECORD_TESTS))

clean ::
	@echo "Clean module record"
	rm -f $(MODDIR_RECORD)/*.o
	rm -f librecord.dll librecord_dll.a librecord.a