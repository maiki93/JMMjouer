#
# First version of library splitting
# Use a dependent module or Makefile ?

# try with module for ccontainer

MODDIR_CCONTAINER := ccontainer

MODULES_TESTS = $(MODDIR_CCONTAINER)/tests
# defined here, used in tests/Module.X.mk
MODDIR_CCONTAINER_TESTS = $(MODDIR_CCONTAINER)/tests

$(info)
$(info == CCONTAINER ==)
$(info $$CURDIR is $(CURDIR) ) 

libccontainer.a : $(MODDIR_CCONTAINER)/clist_generic.o $(MODDIR_CCONTAINER)/clist_cstring.o
	ar rcs $@ $^

# include description for each module
include $(patsubst %,%/Module.mingw64_gcc.mk,$(MODULES_TESTS))
#include $(MODDIR_CONTAINER)/tests/Module.mingw64_gcc.mk

clean ::
	rm -f $(MODDIR_CCONTAINER)/*.o
