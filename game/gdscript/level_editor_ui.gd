extends Control


signal playtest_button_pressed

signal debug_button_pressed
signal test_action_button_pressed(s:String, n: int)
signal undo_button_pressed
signal redo_button_pressed

signal load_level_path_selected(filepath: String)
signal save_level_path_selected(filepath: String)
signal quick_load_level_button_pressed
signal quick_save_level_button_pressed
signal reload_level_button_pressed
signal unload_level_button_pressed

signal tile_flip_button_pressed(mode: int)
signal tile_rotate_button_pressed(clockwise: bool)
signal tile_cycle_button_pressed(next: bool)

signal add_tile_button_pressed(mouse_pos: Vector2)
signal remove_tile_button_pressed(mouse_pos: Vector2)
signal add_player_button_pressed(mouse_pos: Vector2)
signal add_enemy_button_pressed(mouse_pos: Vector2)
signal add_entity_button_pressed(mouse_pos: Vector2)


@export var camera_node: Camera2D
@export var ui_node: Control
@export var help_popup_node: AcceptDialog
@export var load_level_popup_node: FileDialog
@export var save_level_popup_node: FileDialog
@export var bgm_player_node: AudioStreamPlayer
@export var camera_movement_scale: float
@export var camera_movement_multiplier: float

@onready var camera_movement: Vector2 = Vector2(0, 0)


func _process(delta: float) -> void:
	camera_node.translate(camera_movement * delta)
	pass


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("editor_back"):
		_on_back_button_pressed()
		
	if event.is_action_pressed("editor_debug"):
		_on_debug_level_editor_controller_button_pressed()
		_on_debug_scene_manager_button_pressed()
	
	if event.is_action_pressed("editor_camera_up"):
		camera_movement += Vector2(0, -1) * camera_movement_scale
	if event.is_action_released("editor_camera_up"):
		camera_movement += Vector2(0, +1) * camera_movement_scale
		
	if event.is_action_pressed("editor_camera_down"):
		camera_movement += Vector2(0, +1) * camera_movement_scale
	if event.is_action_released("editor_camera_down"):
		camera_movement += Vector2(0, -1) * camera_movement_scale
		
	if event.is_action_pressed("editor_camera_left"):
		camera_movement += Vector2(-1, 0) * camera_movement_scale
	if event.is_action_released("editor_camera_left"):
		camera_movement += Vector2(+1, 0) * camera_movement_scale
		
	if event.is_action_pressed("editor_camera_right"):
		camera_movement += Vector2(+1, 0) * camera_movement_scale
	if event.is_action_released("editor_camera_right"):
		camera_movement += Vector2(-1, 0) * camera_movement_scale
		
	if event.is_action_pressed("editor_camera_slow"):
		camera_movement /= camera_movement_multiplier
		camera_movement_scale /= camera_movement_multiplier
	if event.is_action_released("editor_camera_slow"):
		camera_movement *= camera_movement_multiplier
		camera_movement_scale *= camera_movement_multiplier
		
	if event.is_action_pressed("editor_camera_fast"):
		camera_movement *= camera_movement_multiplier
		camera_movement_scale *= camera_movement_multiplier
	if event.is_action_released("editor_camera_fast"):
		camera_movement /= camera_movement_multiplier
		camera_movement_scale /= camera_movement_multiplier
	
	if event.is_action_pressed("editor_flip_h"):
		_on_tile_flip_h_button_pressed()
	
	if event.is_action_pressed("editor_flip_v"):
		_on_tile_flip_v_button_pressed()
	
	if event.is_action_pressed("editor_flip_d"):
		_on_tile_flip_d_button_pressed()
	
	if event.is_action_pressed("editor_rotate_clockwise"):
		_on_tile_rotate_c_button_pressed()
	
	if event.is_action_pressed("editor_rotate_counter_clockwise"):
		_on_tile_rotate_cc_button_pressed()
	
	if event.is_action_pressed("editor_cycle_previous"):
		_on_tile_cycle_previous_button_pressed()
	
	if event.is_action_pressed("editor_cycle_next"):
		_on_tile_cycle_next_button_pressed()
	
	if event.is_action_pressed("editor_undo"):
		_on_undo_button_pressed()
	
	if event.is_action_pressed("editor_redo"):
		_on_redo_button_pressed()
	
	if event.is_action_pressed("editor_load_level"):
		_on_load_level_button_pressed()
	
	if event.is_action_pressed("editor_save_level"):
		_on_save_level_button_pressed()
	
	if event.is_action_pressed("editor_quick_load_level"):
		_on_quick_load_level_button_pressed()
	
	if event.is_action_pressed("editor_quick_save_level"):
		_on_quick_save_level_button_pressed()
	
	pass


# func _unhandled_key_input(event: InputEvent) -> void:
	# Detect keyboard shortcut inputs
	# pass


func _unhandled_input(event: InputEvent) -> void:
	# Detect non-ui mouse inputs
	# Note: Must set this control node to "Mouse > Filter = Pass"
	if event.is_action_pressed("editor_primary_click"):
		add_tile_button_pressed.emit(self.get_global_mouse_position() + camera_node.get_position())
	
	if event.is_action_pressed("editor_secondary_click"):
		remove_tile_button_pressed.emit(self.get_global_mouse_position() + camera_node.get_position())
		# add_entity_button_pressed.emit(self.get_global_mouse_position() + camera_node.get_position())
	
	pass


func _on_back_button_pressed() -> void:
	SceneManager.get_instance().load_previous_scene(self.get_tree())
	pass


func _on_playtest_button_pressed() -> void:
	playtest_button_pressed.emit()
	pass


func _on_debug_scene_manager_button_pressed() -> void:
	SceneManager.get_instance()._debug()
	pass


func _on_debug_level_editor_controller_button_pressed() -> void:
	debug_button_pressed.emit()
	pass


func _on_test_add_scene_button_pressed() -> void:
	print("Debug: Changing scene to Level Editor (itself)...")
	SceneManager.get_instance().load_new_scene(get_tree(), "res://screen/level_editor.tscn")
	pass


func _on_undo_button_pressed() -> void:
	undo_button_pressed.emit()
	pass


func _on_redo_button_pressed() -> void:
	redo_button_pressed.emit()
	pass


func _on_test_action_1_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 1)
	pass


func _on_test_action_2_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 2)
	pass


func _on_test_action_3_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 3)
	pass


func _on_help_button_pressed() -> void:
	help_popup_node.set_visible(true)
	pass


func _on_load_level_button_pressed() -> void:
	load_level_popup_node.set_visible(true)
	pass


func _on_load_level_popup_file_selected(path: String) -> void:
	load_level_path_selected.emit(path)
	camera_node.set_position(Vector2(0, 0))
	pass


func _on_save_level_button_pressed() -> void:
	save_level_popup_node.set_visible(true)
	pass


func _on_save_level_popup_file_selected(path: String) -> void:
	save_level_path_selected.emit(path)
	pass

func _on_quick_load_level_button_pressed() -> void:
	quick_load_level_button_pressed.emit()
	pass


func _on_quick_save_level_button_pressed() -> void:
	quick_save_level_button_pressed.emit()
	pass


func _on_reload_level_button_pressed() -> void:
	reload_level_button_pressed.emit()
	camera_node.set_position(Vector2(0, 0))
	pass


func _on_unload_level_button_pressed() -> void:
	unload_level_button_pressed.emit()
	camera_node.set_position(Vector2(0, 0))
	pass


func _on_center_level_button_pressed() -> void:
	camera_node.set_position(Vector2(0, 0))
	pass


func _on_music_volume_slider_value_changed(value: float) -> void:
	bgm_player_node.set_volume_db(linear_to_db(value))
	if value < 0.01:
		bgm_player_node.stop()
	elif !bgm_player_node.is_playing():
			bgm_player_node.play()
	pass


func _on_bgm_player_finished() -> void:
	bgm_player_node.play()
	pass


func _on_hide_ui_button_pressed() -> void:
	ui_node.set_visible(!ui_node.is_visible())
	pass


func _on_tile_flip_h_button_pressed() -> void:
	tile_flip_button_pressed.emit(0)
	pass


func _on_tile_flip_v_button_pressed() -> void:
	tile_flip_button_pressed.emit(1)
	pass


func _on_tile_flip_d_button_pressed() -> void:
	tile_flip_button_pressed.emit(2)
	pass


func _on_tile_rotate_c_button_pressed() -> void:
	tile_rotate_button_pressed.emit(true)
	pass


func _on_tile_rotate_cc_button_pressed() -> void:
	tile_rotate_button_pressed.emit(false)
	pass


func _on_tile_cycle_previous_button_pressed() -> void:
	tile_cycle_button_pressed.emit(false)
	pass


func _on_tile_cycle_next_button_pressed() -> void:
	tile_cycle_button_pressed.emit(true)
	pass
