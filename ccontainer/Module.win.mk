
MODDIR_CCONTAINER = ccontainer

OBJS_CCONTAINER = $(MODDIR_CCONTAINER)/clist_generic.obj \
				  $(MODDIR_CCONTAINER)/clist_cstring.obj

OBJS_ALL_STATIC += $(OBJS_CCONTAINER)

$(info)
$(info == CLOGGER ==)

# needed ONLY for passing -Dshared_export now
$(OBJS_CCONTAINER): %.obj: %.c
	@echo "Build file specific rule in ccontainer : $@"
	$(CC) $(WITH_LIB) /Dshared_EXPORTS $(STD) $(CFLAGS) /c $< /Fo"$(dir $@)"

libccontainer_dll.lib : libccontainer.dll

libccontainer.dll : $(OBJS_CCONTAINER) libclogger_dll.lib
	@echo "Create library -- ccontainer"
	$(LINK) $(LFLAGS) /DLL $^ libclogger_dll.lib /IMPLIB:libccontainer_dll.lib /OUT:libccontainer.dll


clean ::
	@echo "Clean module ccontainer"
	del $(MODDIR_CCONTAINER)\*.obj
	del libccontainer.dll, libccontainer_dll.lib , libccontainer_dll.exp