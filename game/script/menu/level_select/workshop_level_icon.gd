extends Control


@export var level_path = ""


func _on_play_button_pressed() -> void:
	SceneManager.get_instance().load_new_scene(get_tree(), level_path)
	# TODO call load_new_scene() with a filepath to a screen, not a level


func _on_edit_button_pressed() -> void:
	var level_editor_node: LevelEditorController = SceneManager.get_instance().import_scene_tscn("res://screen/level_editor.tscn")
	level_editor_node.set_level_filepath(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), "res://screen/level_editor.tscn", level_editor_node)
	pass
