#
# For linux, usual module

MODDIR_CLOGGER = clogger

$(info)
$(info == CLOGGER ==)
$(info $$CURDIR is $(CURDIR) )

OBJS_CLOGGER = $(MODDIR_CLOGGER)/clogger.o

OBJS_ALL_STATIC += $(OBJS_CLOGGER)

libclogger.so : $(OBJS_CLOGGER)
	@echo "Create library -- clogger"
	$(CC) -shared -o $@ $^

# r means to insert with replacement
# c means to create a new archive
# s means to write an index
libclogger.a : $(OBJS_CLOGGER)
	ar rcs $@ $^

## to use for dependencies call
$(info TEST: $(findstring libclogger,$(LIB_STATIC)) )
ifneq ($(findstring libclogger,$(LIB_STATIC)),)
libclogger : libclogger.a
else
# ifneq ($(findstring libclogger,$(LIB_DYNAMIC)),)
libclogger : libclogger.so
#else
#$(info ERROR!!!)
#$(error NO CASE)
endif

#libclogger :
#	ifneq ($(findstring libclogger,$(LIB_STATIC)),)
#		libclogger.a
#	else
#		libclogger.so
#	endif


clean ::
	@echo "Clean module clogger"
	rm -f $(MODDIR_CLOGGER)/*.o
	rm -f libclogger.a libclogger.so
