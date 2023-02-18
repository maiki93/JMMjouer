
# Purpose to create a shared libray
#
#

MODDIR_RECORD := record
# CC, CFLAGS already defined/ shared across Modules
MODULES_TESTS = $(MODDIR_RECORD)/tests

$(info)
$(info == RECORD ==)
$(info $$CURDIR is $(CURDIR) )
$(info $$MODDIR_RECORD_TESTS is $(MODDIR_RECORD_TESTS) )

OBJS_RECORD = $(MODDIR_RECORD)/irecord.o $(MODDIR_RECORD)/file_record.o

# mingw64, must add all dpendencies !! 
# to check on linux
# dependencies on libjoueur and logger
librecord.dll : $(OBJS_RECORD) libclogger.a libjoueur.dll
#	$(CC) -shared $(CFLAGS) -o $@ $^ -L. -lccontainer -L. -ljoueur -Wl,--out-implib,librecord.lib
# working but strange, ok use shared (given by nm)
#	$(CC) -shared $(CFLAGS) -o $@ $^ -Wl,--out-implib,librecord_dll.lib
# more windows, better/more performant to include *_dll.lib
	$(CC) -shared $(CFLAGS) -o $@ $(OBJS_RECORD) libclogger.a libjoueur_dll.lib -Wl,--out-implib,librecord_dll.lib

#librecord.a : $(MODDIR_RECORD)/irecord.o $(MODDIR_RECORD)/file_record.o
#	ar rcs $@ $^

# include module of tests
include $(patsubst %,%/Module.mingw64_gcc.mk,$(MODULES_TESTS))

clean ::
	rm -f $(MODDIR_RECORD)/*.o
	rm -f librecord.dll