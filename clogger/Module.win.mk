

MODDIR_CLOGGER = clogger

$(info)
$(info == CLOGGER ==)
$(info $$CURDIR is $(CURDIR) )

OBJS_CLOGGER = $(MODDIR_CLOGGER)/clogger.obj

OBJS_ALL_STATIC += $(OBJS_CLOGGER)
# use local default to control output directory, found $(dir $@) in root
# needed ONLY for passing -Dshared_export now
$(OBJS_CLOGGER): %.obj: %.c
	@echo "Build file specific rule with header in clogger : $@"
#	$(CC) -Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(MODDIR_CLOGGER)"
	$(CC) -Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

# Windows shared library with *_dll.lib associated
# implib generated only if exported symbol !
# static maybe better in this case, if want to make clogging optional at compilation ?...
libclogger.dll : $(OBJS_CLOGGER)
	@echo "Create library -- clogger"
	$(LINK) $(LFLAGS) /DLL $^ /IMPLIB:libclogger_dll.lib /OUT:libclogger.dll

# allow to use *_dll.lib in dependencies
libclogger_dll.lib : libclogger.dll

clean ::
	@echo "Clean module clogger"
	del $(MODDIR_CLOGGER)\*.obj
	del libclogger.dll, libclogger_dll.lib
