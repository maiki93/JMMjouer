## Define a module to compile the tests of the core
#
# add option -Wno-implicit-function-declaration to suppress warning for "private function mm-algo" (not decalred in a header)

SRCS_JMMJOUER_TESTS := $(wildcard $(MODDIR_JMMJOUER_TESTS)/*.c) # keep full path tests/test_mastermind.c
OBJS_JMMJOUER_TESTS := $(patsubst %.c, %.o, $(SRCS_JMMJOUER_TESTS)) # keep full path tests/test_mastermind.o
# take away extension (patsubst), and all directories  (notdir)
EXE_JMMJOUER_TESTS := $(notdir $(patsubst %.c, %, $(SRCS_JMMJOUER_TESTS) ))

OBJS_ALL_TESTS += $(OBJS_JMMJOUER_TESTS)

$(info == JMMJOUER UNIT TEST : $(MODDIR_JMMJOUER_TESTS) ==)
$(info $$SRCS_JMMJOUER_TESTS is [$(SRCS_JMMJOUER_TESTS) ])
$(info $$OBJS_JMMJOUER_TESTS is [$(OBJS_JMMJOUER_TESTS) ])
$(info $$EXE_JMMJOUER_TESTS is [$(EXE_JMMJOUER_TESTS)] )

# check existence of target(root)/test_X, avoid to rebuild all tests each time called
unit_test:: $(EXE_JMMJOUER_TESTS)

OBJS_NO_ARCADE := $(filter-out jmmjouer/arcade.o, $(OBJS_JMMJOUER))

test_arcade: $(MODDIR_JMMJOUER_TESTS)/test_arcade.o $(OBJS_NO_ARCADE)  librecord libclogger libccontainer libgame_loader libjoueur
	@echo "Building test_arcade @ :    $@"  # target name
	$(CC) $(STD_TESTS) $(CFLAGS_TESTS) -o $@ $(MODDIR_JMMJOUER_TESTS)/test_arcade.o $(OBJS_NO_ARCADE) -L $(LIB_CMOCKA) -lcmocka -L. -l$(IMPORT_LIB_RECORD) -l$(IMPORT_LIB_CLOGGER) -l$(IMPORT_LIB_CCONTAINER) -l$(IMPORT_LIB_GAME_LOADER) -l$(IMPORT_LIB_JOUEUR)

clean::
	@echo "Clean jmmjouer unit tests in : $(MODDIR_JMMJOUER_TESTS)"
	rm -f $(OBJS_JMMJOUER_TESTS)
	rm -f $(EXE_JMMJOUER_TESTS)
