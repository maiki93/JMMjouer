
# Purpose to create a shared libray
#
#

MODDIR_RECORD := record

# CC, CFLAGS already defined/ shared with Modules

$(info)
$(info == RECORD ==)
$(info$$CURDIR is $(CURDIR) )

# mingw64, must add all dpendencies !!
# to check on linux
librecord.dll : $(MODDIR_RECORD)/irecord.o $(MODDIR_RECORD)/file_record.o libclogger.a #person.o joueur.o cmap_game_victories.o
	$(CC) -shared $(CFLAGS) -o $@ $^ -L. -lccontainer -L. -ljoueur -Wl,--out-implib,librecord.lib

#librecord.a : $(MODDIR_RECORD)/irecord.o $(MODDIR_RECORD)/file_record.o
#	ar rcs $@ $^

clean ::
	rm -f $(MODDIR_RECORD)/*.o