extends Control


signal open_level_editor


func _on_open_level_editor_button_pressed() -> void:
	open_level_editor.emit()
	pass
