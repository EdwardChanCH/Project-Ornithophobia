extends GutTest


func test_scene_manager_get_instance() -> void:
	var scene_manager: SceneManager = SceneManager.get_instance()
	var scene_manager_another = SceneManager.get_instance()
	
	assert_same(scene_manager, scene_manager_another, "SceneManager should only have 1 instance.")
	
	pass # End of function


func test_scene_manager_import_scene_tscn() -> void:
	var scene_manager: SceneManager = SceneManager.get_instance()
	var scene_node: Node = scene_manager.import_scene_tscn("res://screen/edward_main.tscn")
	
	assert_not_null(scene_node)
	assert_is(scene_node, Node, "SceneManager should return a scene node.")
	
	scene_node.free()
	
	pass # End of function


func test_scene_manager_load_new_scene_node() -> void:
	var scene_manager: SceneManager = SceneManager.get_instance()
	
	assert_false(scene_manager.load_new_scene_node(null, "res://invalid/path", null))
	
	# assert_true(scene_manager.load_new_scene_node(get_tree(), "res://screen/edward_main.tscn", Node.new()))
	# ^ Note: Cannot test this since GUT uses the SceneTree as well!
	
	pass # End of function


func test_scene_manager_load_new_scene() -> void:
	var scene_manager: SceneManager = SceneManager.get_instance()
	
	assert_false(scene_manager.load_new_scene(null, "res://invalid/path"))
	
	# assert_true(scene_manager.load_new_scene(get_tree(), "res://screen/edward_main.tscn"))
	# ^ Note: Cannot test this since GUT uses the SceneTree as well!
	
	pass # End of function


func test_scene_manager_load_previous_scene() -> void:
	var scene_manager: SceneManager = SceneManager.get_instance()
	
	assert_false(scene_manager.load_previous_scene(get_tree()))
	
	# scene_manager.load_new_scene(get_tree(), "res://screen/edward_main.tscn")
	# assert_true(scene_manager.load_previous_scene(get_tree()))
	# ^ Note: Cannot test this since GUT uses the SceneTree as well!
	
	pass # End of function
