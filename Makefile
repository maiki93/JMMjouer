# many tricks
# https://stackoverflow.com/questions/53136024/makefile-to-compile-all-c-files-without-needing-to-specify-them

# debug option,
# yes, use in code of DEBUG_CODE (example in main.c), and -g four building in debug mode( possible to use a debugger)
# no, compile with optimization -O2
debug = yes
#debug = no

# compilers, may give different errors/warning
CC = gcc
#CC = clang

# List of subdirectories containing a Module.mk file
MODULES := tests


# compiler option, activate many warning( here seeems common to gcc and clang)
OPTION_CC = -W -Wall -fPIC -Wunused -Wextra -pedantic -Wstrict-overflow=5 -fno-inline -Wno-unused-local-typedefs

ifeq ($(debug),yes)          # separator mandatory between ifeq and '('
	OPTION_CC += -g -DDEBUG_CODE
else
	OPTION_CC += -O2
endif

# search for all *.c files and make *.o files
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
EXE  := JMMjouer

# declare all targets
.PHONY : all unit_test clean

# make all, generic way to use make produce executable JMMjouer
all: $(EXE)
# specific target for unit testing in tests
unit_test : test_mastermind

# general rule for compiling c files
%.o:  %.c
	@echo "Build file:   $@"
	$(CC) $(OPTION_CC) -c $< -o $@

# explicit call, not very nice and extendable
#all: utils.o main.o historique.o game_mastermind.o game_pendu.o morpion.o
#	$(CC) -o JMMjouer utils.o main.o historique.o game_mastermind.o game_pendu.o morpion.o

$(EXE): $(OBJS)
	$(CC) $(OPTION_CC) $^ -o $@ 

# include the description for each module
include $(patsubst %,%/Module.mk,$(MODULES))

# call clean here and in subdirectories
clean::
	@echo "Clean in root directory"
	rm -f ./*.o
	rm -f JMMjouer
