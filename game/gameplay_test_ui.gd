extends Control


signal back_button_pressed
signal unload_button_pressed
signal reload_button_pressed
signal load_button_pressed
signal save_button_pressed


func _on_back_button_pressed() -> void:
	back_button_pressed.emit()
	pass


func _on_unload_button_pressed() -> void:
	unload_button_pressed.emit()
	pass


func _on_reload_button_pressed() -> void:
	reload_button_pressed.emit()
	pass


func _on_load_button_pressed() -> void:
	load_button_pressed.emit()
	pass


func _on_save_button_pressed() -> void:
	save_button_pressed.emit()
	pass
