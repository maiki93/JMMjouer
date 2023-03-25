#
# For linux, usual module
#
# File format
# objdump -f clogger/clogger.o
#
# clogger/clogger.o:     file format elf64-x86-64
# architecture: i386:x86-64, flags 0x00000011:
# HAS_RELOC, HAS_SYMS
# start address 0x0000000000000000

# libclogger.so:     file format elf64-x86-64
# architecture: i386:x86-64, flags 0x00000150:
# HAS_SYMS, DYNAMIC, D_PAGED
# start address 0x0000000000000000

MODDIR_CLOGGER := clogger

SRCS_CLOGGER := $(wildcard $(MODDIR_CLOGGER)/*.c)
OBJS_CLOGGER := $(patsubst %.c, %.o, $(SRCS_CLOGGER))

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

test_clogger : $(MODDIR_CLOGGER)/tests/test_clogger.o $(MODDIR_CLOGGER)/clogger.o
	@echo "Building test_clogger @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS) $? -o $@ -L $(LIB_CMOCKA) -lcmocka


clean ::
	@echo "Clean module clogger"
#	rm -f $(OBJS_CLOGGER)
	rm -f $(MODDIR_CLOGGER)/tests/test_clogger.o
	rm -f libclogger.a libclogger.so
	rm -f test_clogger
