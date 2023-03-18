
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
librecord.dll : $(OBJS_RECORD) libjoueur #libclogger.lib libjoueur.dll
# -l libjoueur.dll is enought, no need explicit clogger or ccontainer
#	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_RECORD) -Wl,--out-implib,librecord_dll.a -L. -ljoueur
# same !! is because libjoueur included statically ? chance ? nm -gC no undefined a priori 
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_RECORD) -Wl,--out-implib,librecord_dll.a -L. -ljoueur_dll
#	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_RECORD) libclogger.a libjoueur_dll.lib -Wl,--out-implib,librecord_dll.lib

librecord.a : $(OBJS_RECORD)
	ar rcs $@ $^

ifneq ($(findstring librecord,$(LIB_STATIC)),)
librecord : librecord.a
else
librecord : librecord.dll
endif

# include module of tests
include $(patsubst %,%/Module_test.mingw64_gcc.mk,$(MODDIR_RECORD_TESTS))

clean ::
	@echo "Clean module record"
	rm -f $(MODDIR_RECORD)/*.o
	rm -f librecord.dll librecord_dll.a librecord.a