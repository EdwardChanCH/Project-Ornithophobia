extends Control


@export var level_path = ""


func _on_play_button_pressed() -> void:
	SceneManager.get_instance().load_new_scene(get_tree(), level_path)


func _on_edit_button_pressed() -> void:
	# Open level editor
	pass # Replace with function body.
