# Author: Jacob Couture
# Description: Controller for the level banners in the Workshop screen

extends Control


@export var level_path = ""
const LEVEL_CONTROLLER_PATH = "res://level/level_controller.tscn"


# Play associated level when play button pressed
func _on_play_button_pressed() -> void:
	var level_controller: LevelController = SceneManager.get_instance().import_scene_tscn(LEVEL_CONTROLLER_PATH)
	level_controller.set_level(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), LEVEL_CONTROLLER_PATH, level_controller)


# Open level editor on associated level when edit button pressed
func _on_edit_button_pressed() -> void:
	var level_editor_node: LevelEditorController = SceneManager.get_instance().import_scene_tscn("res://screen/level_editor.tscn")
	level_editor_node.set_level_filepath(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), "res://screen/level_editor.tscn", level_editor_node)
	pass
