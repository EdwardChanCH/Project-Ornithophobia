extends Control


func _on_resume_button_pressed() -> void:
	visible = false
	get_tree().paused = false


func _on_return_to_menu_button_pressed() -> void:
	get_tree().paused = false
	visible = false
	SceneManager.get_instance().load_new_scene(get_tree(), "res://screen/menu/level_select.tscn")


func _on_exit_button_pressed() -> void:
	get_tree().paused = false
	get_tree().quit()
