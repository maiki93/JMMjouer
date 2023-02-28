
MODDIR_RECORD := record

SRCS_RECORD := $(wildcard $(MODDIR_RECORD)/*.c)
OBJS_RECORD := $(patsubst %.c, %.o, $(SRCS_RECORD))

# update global variable
OBJS_ALL_STATIC += $(OBJS_RECORD)

# define tests file
MODDIR_RECORD_TESTS = $(MODDIR_RECORD)/tests

$(info == RECORD ==)
$(info $$SRCS_RECORD is [$(SRCS_RECORD)] )
$(info $$OBJS_RECORD is [$(OBJS_RECORD)] )


librecord.so : $(OBJS_RECORD) libclogger libccontainer libjoueur
	@echo "Create shared library -- record"
	$(CC) -shared -o $@ $(OBJS_RECORD) -L . -ljoueur -lclogger -lccontainer 

#librecord.a : $(OBJS_RECORD) libclogger.so libccontainer.so
librecord.a : $(OBJS_RECORD)
	@echo "Create static library -- record"
	ar rcs $@ $^

ifneq ($(findstring librecord,$(LIB_STATIC)),)
librecord : librecord.a
else
librecord : librecord.so
endif

include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_RECORD_TESTS))

clean ::
	@echo "Clean module record"
	rm -f $(OBJS_RECORD)
	rm -f librecord.so librecord.a

