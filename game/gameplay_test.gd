extends Node2D


var level_node: Node = null


func _enter_tree() -> void:
	level_node = null
	pass


func _exit_tree() -> void:
	unload_level()
	pass


func unload_level() -> void:
	if level_node != null:
		level_node.queue_free()
		level_node = null
	pass


func load_level(filepath: String) -> void:
	unload_level()
	level_node = ResourceLoader.load(filepath, "PackedScene", ResourceLoader.CACHE_MODE_REUSE).instantiate()
	add_child(level_node)
	pass


func save_level(filepath: String) -> void:
	var scene: PackedScene
	
	if (level_node != null) and (filepath != ""):
		scene = PackedScene.new()
		scene.pack(level_node)
		ResourceSaver.save(scene, filepath, 0)
	pass


func _on_gameplay_test_ui_back_button_pressed() -> void:
	SceneManager.get_instance().load_previous_scene(get_tree())
	pass


func _on_gameplay_test_ui_unload_button_pressed() -> void:
	unload_level()
	pass
	

func _on_gameplay_test_ui_reload_button_pressed() -> void:
	load_level("res://level_test.tscn")
	pass


func _on_gameplay_test_ui_load_button_pressed() -> void:
	load_level("res://test_scene_001.tscn")
	pass


func _on_gameplay_test_ui_save_button_pressed() -> void:
	save_level("res://test_scene_001.tscn")
	pass
