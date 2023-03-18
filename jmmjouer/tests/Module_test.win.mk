## Define a module to compile the tests
#
# version for msvc compilation
#
#MODDIR_JMMJOUER_TESTS := jmmjouer\tests

SRCS_JMMJOUER_TESTS := $(wildcard $(MODDIR_JMMJOUER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JMMJOUER_TESTS := $(patsubst %.c, %.obj, $(SRCS_JMMJOUER_TESTS)) # keep full path tests/test_mastermind.o
# take away extension (patsubst), and all directories  (notdir)
EXE_JMMJOUER_TESTS_BIN := $(notdir $(patsubst %.c, %, $(SRCS_JMMJOUER_TESTS) ))

OBJS_ALL_TESTS += $(OBJS_JMMJOUER_TESTS)

$(info == JMMJOUER UNIT TEST : $(MODDIR_JMMJOUER_TESTS) ==)
$(info $$SRCS_JMMJOUER_TESTS is [$(SRCS_JMMJOUER_TESTS) ])
$(info $$OBJS_JMMJOUER_TESTS is [$(OBJS_JMMJOUER_TESTS) ])
$(info $$EXE_JMMJOUER_TESTS is [$(EXE_JMMJOUER_TESTS)] )

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_JMMJOUER_TESTS)

OBJS_NO_ARCADE := $(filter-out jmmjouer/arcade.obj, $(OBJS_JMMJOUER))

# game_pendu.obj not needed anymore to compile
test_arcade: $(MODDIR_JMMJOUER_TESTS)/test_arcade.obj $(OBJS_NO_ARCADE) \
		$(IMPORT_LIB_GAME_LOADER) $(IMPORT_LIB_RECORD) $(IMPORT_LIB_JOUEUR) $(IMPORT_LIB_CCONTAINER) $(IMPORT_LIB_CLOGGER)
	@echo "Building test_arcade @ :    $@"  # target name
#	$(LINK) $(LFLAGS) /NOIMPLIB /NOEXP /LIBPATH:$(LIB_CMOCKA) cmocka.lib $?
	$(LINK) $(LFLAGS) $? /LIBPATH:$(LIB_CMOCKA) cmocka.lib

clean::
	@echo "Clean unit tests in $(MODDIR_JMMJOUER_TESTS) "
	del $(MODDIR_JMMJOUER_TESTS)\*.obj
	del test_*.exe
