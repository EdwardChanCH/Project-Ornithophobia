extends Control


func _on_back_button_pressed() -> void:
	SceneManager.get_instance().load_previous_scene(get_tree())
	pass
