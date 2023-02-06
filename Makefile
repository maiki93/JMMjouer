# version linux (tested wsl2/ubuntu)
# use gcc, very similar except: 
#	- standard name of libraries
# + code modification (windows / posix ), visibility of variable in object file / library 

# many tricks
# https://stackoverflow.com/questions/53136024/makefile-to-compile-all-c-files-without-needing-to-specify-them
# https://accu.org/journals/overload/14/71/miller_2004/ famous paper: recursive make considered harmful

# debug option, override with make all debug=no CFLAGS=-std=c99
# yes, use in code of JMMJ_DEBUG (example in main.c), and -g four building in debug mode( possibility to use a debugger)
# no, compile with optimization -O2
debug = yes
#debug = no

# compilers, may give different errors/warning
CC = gcc
#CC = clang

# List of subdirectories containing a Module.mk file
#MODULES := tests tests/test_mock
MODULES := tests

# see advices (and much more than compilation options) https://matt.sh/howto-c
# compiler option, activate many warning( here seeems common to gcc and clang)
# pedantic => -Wpedantic gcc new
# -fno-inline ? 
# -Wno-unused-local-typedefs no need
# -Wstrict-overflow=5, make pass game_morpion compilation
# -Wunused -W (included already)
# -fPIC on Windows, not adviced ? 
#       on linux not for performance ? but cannot comile, give error !
# -g3 : all debug information + MACRO (-g0 : no debug ), -g default 2
# -ggdb : include gdb extension if possible (not tested)
CFLAGS = -Wall -Wextra -pedantic -fPIC -Wno-variadic-macros #-Wno-strict-overflow
# to include again:  -Werror, better than Wno-X warning appears but compiles fine

# standard configurable
# gcc c89/90 : -ansi, -std=c90 or -std=iso9899:1990
STD = -std=c90

# standard gcc defines usually NDEBUG for release code (assert.h affected)
# code uses internally a JMMJ_DEBUG, defined in preprocessor stage 
# 	or declare directly here ? better will apply to all files without need to include a common header ?
ifeq ($(debug),yes)          # separator mandatory between ifeq and '('
# use specific code for the project, g3 to include MACRO definition as variables
	CFLAGS += -g3 -O0 -DJMMJ_DEBUG
else
# kind of standard with gcc projects
	CFLAGS += -DNDEBUG -O2
endif

$(info $$CC is $(CC) )
$(info $$CFLAGS is $(CFLAGS) )

# Use explicit OBJS, more control
# do not include main.o
OBJS = clogger.o utils.o utils_file.o \
	   clist_generic.o clist_cstring.o  \
	   victory.o cmap_game_victories.o person.o joueur.o \
	   plugin_manager.o cmap_ptrf_game.o game_loader.o \
	   game_mastermind.o \
	   file_record.o irecord.o \
	   arcade.o

# define main executable
EXE  := JMMjouer

# inspect variables
$(info $$OBJS is [$(OBJS)] )

# alternative to target as files
.PHONY : all clean unit_test

# make all, generic way to use make produce executable JMMjouer
all: $(EXE)
# specific target for unit testing in tests/ , dependencies with CMocka
unit_test::

# general rule for compiling c files when header file is available 
%.o: %.c %.h
	@echo "Build file generic rule with header in root:   $@"
	$(CC) $(STD) $(CFLAGS) -c $< -o $@

# general rule, no header file associated (ex. main.c)
%.o: %.c
	@echo "Build file generic rule without header in root:   $@"
	$(CC) $(STD) $(CFLAGS) -c $< -o $@

# -Wl,-rpath,dir1  or -rpath=dir1 # -Wl to pass argument to linker, rpath runtime to search for lib
$(EXE): $(OBJS) main.o libgame_pendu.so
	@echo "Build $(EXE): all dependencies $^"
	$(CC) $(STD) $(CFLAGS) $^ -L. -lgame_pendu -o $@

##### compile shared library to include at compile-time
# linux, ask only for shared or static
libgame_pendu.so : game_pendu.o game_pendu.h victory.o
	$(CC) -shared $(CFLAGS) $^ -o $@

##### compile shared library to include at run-time (game_morpion)
# version unix-like available more recently in mingw
#libmorpion.dll : game_morpion.o game_morpion.h
#	$(CC) -shared game_morpion.o -o libmorpion.dll

# windows style, with importing lib, not used at run-time link anyway
libgame_morpion.so : game_morpion.o game_morpion.h victory.o
	$(CC) -shared $(CFLAGS) $^ -o $@

# include the description for each module
include $(patsubst %,%/Module.mk,$(MODULES))

# call clean here and in subdirectories
clean::
	@echo "Clean in root directory"
	rm -f ./*.o
	rm -f ./*.so ./*.a
	rm -f JMMjouer
