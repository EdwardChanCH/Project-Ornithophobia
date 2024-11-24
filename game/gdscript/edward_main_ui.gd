extends Control


signal open_level_editor_screen
@export var test: Level


func _on_open_level_editor_button_pressed() -> void:
	open_level_editor_screen.emit()
	pass
