extends Control


@export var level_path = ""
var level_controller_path = "res://level/level_controller.tscn"


func _on_play_button_pressed() -> void:
	var level_controller: LevelController = SceneManager.get_instance().import_scene_tscn(level_controller_path)
	level_controller.set_level(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), level_controller_path, level_controller)


func _on_edit_button_pressed() -> void:
	var level_editor_node: LevelEditorController = SceneManager.get_instance().import_scene_tscn("res://screen/level_editor.tscn")
	level_editor_node.set_level_filepath(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), "res://screen/level_editor.tscn", level_editor_node)
	pass
