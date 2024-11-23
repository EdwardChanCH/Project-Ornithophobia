extends Control


signal open_level_editor_screen


func _on_open_level_editor_button_pressed() -> void:
	open_level_editor_screen.emit()
	pass
