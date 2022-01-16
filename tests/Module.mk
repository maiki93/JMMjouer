
# define a module to compile the tests
MODDIR_C := tests

# if give dpendenciens, but building rules ../game_mastermind.o does not work
# ok, everything is imported in root directory
test_mastermind : game_mastermind.o utils.o
	echo "Building unit tests :    $@"
	gcc -Wno-implicit-function-declaration $(MODDIR_C)/test_mastermind.c game_mastermind.o utils.o -l cmocka -o test_mastermind

clean::
	@echo "Clean unit tests in MODDIR_C = $(MODDIR_C) "
	rm -f $(MODDIR_C)/*.o
	rm -f test_mastermind
