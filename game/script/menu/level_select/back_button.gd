extends Button


func _on_pressed() -> void:
	# get_tree().change_scene_to_file("res://screen/menu/main_menu.tscn")
	SceneManager.get_instance().load_previous_scene(get_tree())
	pass
