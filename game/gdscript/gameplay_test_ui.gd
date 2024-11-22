extends Control


signal back_button_pressed
signal unload_button_pressed
signal reload_button_pressed
signal load_button_pressed
signal save_button_pressed
signal screen_left_clicked
signal screen_right_clicked


func _unhandled_key_input(event: InputEvent) -> void:
	# For detecting keyboard shortcut inputs
	pass


func _unhandled_input(event: InputEvent) -> void:
	# For detecting non-ui mouse inputs
	# Note: Must set root control node to "Mouse > Filter = Pass"
	
	if event.is_action_pressed("left_click"):
		screen_left_clicked.emit()
	if event.is_action_pressed("right_click"):
		screen_right_clicked.emit()
	pass


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
