
MODDIR_CCONTAINER := ccontainer

SRCS_CCONTAINER := $(wildcard $(MODDIR_CCONTAINER)/*.c)
OBJS_CCONTAINER := $(patsubst %.c, %.o, $(SRCS_CCONTAINER))
# update global variable
OBJS_ALL_STATIC += $(OBJS_CCONTAINER)

# defined here, used in tests/Module.X.mk
MODDIR_CCONTAINER_TESTS = $(MODDIR_CCONTAINER)/tests

$(info == CCONTAINER : $(MODDIR_CCONTAINER) ==)
$(info $$SRCS_CCONTAINER is [$(SRCS_CCONTAINER)] )
$(info $$OBJS_CCONTAINER is [$(OBJS_CCONTAINER)] )

libccontainer.so : $(OBJS_CCONTAINER) libclogger
	@echo "Create shared library -- ccontainer"
	$(LINK) $(LFLAGS) -o $@ $(OBJS_CCONTAINER) -L . -lclogger
#	$(CC) -shared -o $@ $(OBJS_CCONTAINER) -L . -lclogger

libccontainer.a : $(OBJS_CCONTAINER)
	echo "Create static library -- ccontainer"
	ar rcs $@ $^

ifneq ($(findstring libccontainer,$(LIB_STATIC)),)
libccontainer : libccontainer.a
else
libccontainer : libccontainer.so
endif

# include description for each module
include $(patsubst %,%/Module_test.linux.mk,$(MODDIR_CCONTAINER_TESTS))

clean ::
	@echo "Clean module ccontainer"
	rm -f $(MODDIR_CCONTAINER)/*.o
	rm -f libccontainer.so libccontainer.a