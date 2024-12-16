extends GutTest

func test_scene_manager_get_instance() -> void:
	# Pretend this result is from a function call
	var result: SceneManager = SceneManager.get_instance()
	
	assert_eq(result.get_instance_id(), SceneManager.get_instance().get_instance_id(), "SceneManager should only have 1 instance.")
	
	pass # End of function
