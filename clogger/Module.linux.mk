#
# For linux, usual module

MODDIR_CLOGGER := clogger

SRCS_CLOGGER := $(wildcard $(MODDIR_CLOGGER)/*.c)
OBJS_CLOGGER := $(patsubst %.c, %.o, $(SRCS_CLOGGER))
#OBJS_CLOGGER = $(MODDIR_CLOGGER)/clogger.o

OBJS_ALL_STATIC += $(OBJS_CLOGGER)

$(info)
$(info == CLOGGER : $(MODDIR_CLOGGER) ==)
$(info $$SRCS_CLOGGER is [$(SRCS_CLOGGER)] )
$(info $$OBJS_CLOGGER is [$(OBJS_CLOGGER)] )

libclogger.so : $(OBJS_CLOGGER)
	@echo "Create shared library -- clogger"
#	$(CC) -shared -o $@ $^
	$(LINK) $(LFLAGS) -o $@ $^

# r means to insert with replacement
# c means to create a new archive
# s means to write an index
libclogger.a : $(OBJS_CLOGGER)
	@echo "Create static library -- clogger"
	ar rcs $@ $^

## to use for dependencies call
$(info TEST: $(findstring libclogger,$(LIB_STATIC)) )
ifneq ($(findstring libclogger,$(LIB_STATIC)),)
libclogger : libclogger.a
else
libclogger : libclogger.so
endif

#### TU
OBJS_ALL_TESTS += $(MODDIR_CLOGGER)/tests/test_clogger.o

unit_test :: test_clogger


clean ::
	@echo "Clean module clogger"
	rm -f $(MODDIR_CLOGGER)/*.o
	rm -f libclogger.a libclogger.so
