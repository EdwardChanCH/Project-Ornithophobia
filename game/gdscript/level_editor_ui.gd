extends Control


signal open_previous_screen
signal playtest_button_toggled(active: bool)
signal debug_level_editor_controller
signal test_action_button_pressed(n: int)
signal undo_button_pressed
signal redo_button_pressed
signal load_level_path_selected(filepath: String)
signal save_level_path_selected(filepath: String)
signal reload_level_button_pressed
signal unload_level_button_pressed
signal screen_left_clicked(mouse_pos: Vector2)
signal screen_right_clicked(mouse_pos: Vector2)


@export var load_level_popup_node: FileDialog
@export var save_level_popup_node: FileDialog


# func _unhandled_key_input(event: InputEvent) -> void:
	# Detect keyboard shortcut inputs
	# pass


func _unhandled_input(event: InputEvent) -> void:
	# Detect non-ui mouse inputs
	# Note: Must set this control node to "Mouse > Filter = Pass"
	
	if event.is_action_pressed("left_click"):
		screen_left_clicked.emit(get_viewport().get_mouse_position())
	if event.is_action_pressed("right_click"):
		screen_right_clicked.emit(get_viewport().get_mouse_position())
	pass


func test_print(s: String) -> void:
	print("Do Action: ", s)
	pass


func test_unprint(s: String) -> void:
	print("Undo Action: ", s)
	pass


func _on_back_button_pressed() -> void:
	open_previous_screen.emit()
	pass


func _on_playtest_button_toggled(toggled_on: bool) -> void:
	playtest_button_toggled.emit(toggled_on)
	pass


func _on_debug_scene_manager_button_pressed() -> void:
	SceneManager.get_instance()._debug()
	pass


func _on_debug_level_editor_controller_button_pressed() -> void:
	debug_level_editor_controller.emit()
	pass


func _on_test_add_scene_button_pressed() -> void:
	print("Changing scene to Level Editor (itself)...")
	SceneManager.get_instance().load_new_scene(get_tree(), "res://screen/level_editor.tscn")
	pass


func _on_undo_button_pressed() -> void:
	undo_button_pressed.emit()
	pass


func _on_redo_button_pressed() -> void:
	redo_button_pressed.emit()
	pass


func _on_test_action_1_button_pressed() -> void:
	test_action_button_pressed.emit(1)
	pass


func _on_test_action_2_button_pressed() -> void:
	test_action_button_pressed.emit(2)
	pass


func _on_test_action_3_button_pressed() -> void:
	test_action_button_pressed.emit(3)
	pass


func _on_load_level_button_pressed() -> void:
	load_level_popup_node.set_visible(true)
	pass


func _on_load_level_popup_file_selected(path: String) -> void:
	load_level_path_selected.emit(path)
	print("Load File Selected: ", path)
	pass


func _on_save_level_button_pressed() -> void:
	save_level_popup_node.set_visible(true)
	pass


func _on_save_level_popup_file_selected(path: String) -> void:
	save_level_path_selected.emit(path)
	print("Save File Selected: ", path)
	pass


func _on_reload_level_button_pressed() -> void:
	reload_level_button_pressed.emit()
	pass


func _on_unload_level_button_pressed() -> void:
	unload_level_button_pressed.emit()
	pass
