extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_back_button_pressed() -> void:
	print("Changing scene to EdwardMain...")
	SceneManager.get_instance().load_previous_scene(get_tree())
	# get_tree().change_scene_to_file("res://edward_main.tscn")


func _on_debug_scene_manager_pressed() -> void:
	SceneManager.get_instance()._debug()


func _on_test_add_scene_pressed() -> void:
	print("Adding scene to stack...")
	SceneManager.get_instance().load_new_scene(get_tree(), "res://level_editor_ui.tscn")
