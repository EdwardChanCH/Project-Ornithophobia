class_name EdwardMainUI
extends Control
## This class represents EdwardMain's UI.
##
## This script aggregates all UI signals and updates all UI elements.
##
## Author: Chun Ho Chan (Edward)
## Date:   2024-12-13


signal open_level_editor_screen


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("debug"):
		print("DEBUG")
		SceneManager.get_instance()._debug()
	pass


func _on_open_level_editor_button_pressed() -> void:
	open_level_editor_screen.emit()
	pass
