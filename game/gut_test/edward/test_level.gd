extends GutTest


func test_level_constructor() -> void:
	var level: Level = partial_double(Level).new()
	
	assert_not_null(level)
	
	pass # End of function


func test_level_constructor_copy() -> void:
	var level: Level = partial_double(Level).new()
	var level_info: Dictionary = Dictionary()
	level_info["key1"] = "value1"
	level.set_level_info(level_info)
	
	var level_copy: Level = level.clone()
	
	assert_not_null(level_copy)
	assert_eq_deep(level_info, level_copy.get_level_info())
	
	level_copy.free()
	
	pass # End of function


func test_level_set_get_level_info() -> void:
	var level: Level = partial_double(Level).new()
	var level_info: Dictionary = Dictionary()
	level_info["key1"] = "value1"
	level.set_level_info(level_info)
	var level_info_r = level.get_level_info()
	
	assert_eq_deep(level_info, level_info_r)
	
	pass # End of function


func test_level_set_physics() -> void:
	var level: Level = partial_double(Level).new()
	level.set_physics(false)
	
	assert_eq(PROCESS_MODE_DISABLED, level.get_process_mode())
	
	pass # End of function


func test_level_add_get_list() -> void:
	var level: Level = partial_double(Level).new()
	
	assert_true(level.add_list("TempList"))
	assert_not_null(level.get_list("TempList"))
	
	pass # End of function


func test_level_add_node_to_list() -> void:
	var level: Level = partial_double(Level).new()
	var temp_node: Node = partial_double(Node).new()
	
	level.add_list("TempList")
	level.add_node_to_list("TempList", temp_node)
	assert_eq(1, level.get_list_length("TempList"))
	
	pass # End of function


func test_level_clear_list() -> void:
	var level: Level = partial_double(Level).new()
	var temp_node: Node = partial_double(Node).new()
	
	level.add_list("TempList")
	level.add_node_to_list("TempList", temp_node)
	level.clear_list("TempList")
	assert_eq(0, level.get_list_length("TempList"))
	
	pass # End of function


func test_level_get_list_length() -> void:
	var level: Level = partial_double(Level).new()
	var temp_node: Node = partial_double(Node).new()
	
	level.add_list("TempList")
	assert_eq(0, level.get_list_length("TempList"))
	level.add_node_to_list("TempList", temp_node)
	assert_eq(1, level.get_list_length("TempList"))
	
	pass # End of function


func test_level_get_node_in_list() -> void:
	var level: Level = partial_double(Level).new()
	var temp_node: Node = partial_double(Node).new()
	temp_node.set_name("TempNode")
	
	level.add_list("TempList")
	level.add_node_to_list("TempList", temp_node)
	assert_not_null(level.get_node_in_list("TempList", "TempNode"))
	
	pass # End of function


func test_level_get_node_in_list_by_index() -> void:
	var level: Level = partial_double(Level).new()
	var temp_node: Node = partial_double(Node).new()
	
	level.add_list("TempList")
	level.add_node_to_list("TempList", temp_node)
	assert_not_null(level.get_node_in_list_by_index("TempList", 0))
	
	pass # End of function
