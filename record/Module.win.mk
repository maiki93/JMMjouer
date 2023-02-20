
# Just replace JOUEUR / RECORD && joueur / record
MODDIR_RECORD = record

SRCS_RECORD := $(wildcard $(MODDIR_RECORD)/*.c)
OBJS_RECORD = $(patsubst %.c, %.obj, $(SRCS_RECORD))

# update global variable
OBJS_ALL_STATIC += $(OBJS_RECORD)

$(info == RECORD ==)
$(info $$SRCS_RECORD is [$(SRCS_RECORD)] )
$(info $$OBJS_RECORD is [$(OBJS_RECORD)] )

# needed ONLY for passing -Dshared_export now
$(OBJS_RECORD): %.obj: %.c
	@echo "Build file specific rule in ccontainer : $@"
	$(CC) -Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"


#libclogger_dll.lib needed ? not imported by joueur ? (little child)
librecord.dll : $(OBJS_RECORD) libjoueur_dll.lib libclogger_dll.lib
	@echo "Create shared library -- record"
	$(LINK) $(LFLAGS) /DLL $^  /IMPLIB:librecord_dll.lib /OUT:librecord.dll

librecord_dll.lib : librecord.dll

# Static library
#librecord.lib : $(OBJS_RECORD)
#	@echo "Create static library -- librecord"
#	link.exe -lib $^ /OUT:libgame_loader.lib
#	$(LINK) /lib /OUT:librecord.lib $^

clean ::
	@echo "Clean module record"
	del $(MODDIR_RECORD)\*.obj
	del librecord.dll, librecord_dll.lib
