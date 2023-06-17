#
# File created manually, 
# Allow to use the CTest tool to combine multiple executable
# Create output in default Testing\Temporary\LastTest.log

# specific makefile, where best to make test ? in jmmjouer/tests ?
# add_test (test_clogger "test_clogger")
add_test (test_clist_generic "test_clist_generic")
add_test (test_clist_cstring "test_clist_cstring")
add_test (test_cvector_generic "test_cvector_generic")
add_test (test_cvector_cstring "test_cvector_cstring")
add_test (test_cvector_struct_complex "test_cvector_struct_complex")
add_test (test_map_game_ptrf "test_map_game_ptrf")
add_test (test_map_game_score "test_map_game_score")
add_test (test_joueur "test_joueur")
add_test (test_joueur "test_ccontainer_joueur")
add_test (test_mastermind "test_mastermind")
add_test (test_file_record "test_file_record")
add_test (test_game_loader "test_game_loader")
add_test (test_plugin_manager "test_plugin_manager")
# need mock, ask for console entry
#add_test (test_arcade "test_arcade.exe")
