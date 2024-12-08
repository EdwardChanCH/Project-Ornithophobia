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


@export var load_level_popup_node: FileDialog
@export var save_level_popup_node: FileDialog
@export var help_popup_node: AcceptDialog
@export var debug_popup_node: AcceptDialog

@export var camera_node: Camera2D
@export var menu_node: Control
@export var grid_layer: ParallaxLayer
@export var edit_mode_node: OptionButton
@export var bgm_player_node: AudioStreamPlayer
@export var ghost_tile_node: Tile2D
@export var ghost_player_node: Node2D
@export var ghost_enemy_node: Node2D
@export var ghost_orb_node: Node2D

@export var camera_movement_scale: float
@export var camera_movement_multiplier: float
@onready var camera_movement: Vector2 = Vector2(0, 0)


func _ready() -> void:
	# Receiver parent signals
	get_parent().get_parent().connect("level_saved", Callable(self, "_on_level_saved"))
	get_parent().get_parent().connect("tile_type_changed", Callable(self, "_on_tile_type_changed"))
	get_parent().get_parent().connect("tile_alt_changed", Callable(self, "_on_tile_alt_changed"))
	
	# Create directories
	DirAccess.make_dir_recursive_absolute("user://level")
	DirAccess.make_dir_recursive_absolute("user://level/story")
	DirAccess.make_dir_recursive_absolute("user://level/user")
	DirAccess.make_dir_recursive_absolute("user://level/workshop")
	
	# First-time setups
	_on_edit_mode_button_item_selected(edit_mode_node.selected)
	
	pass


func _process(delta: float) -> void:
	# Move camera
	camera_node.translate(camera_movement * delta)
	
	# Move edit mode indicators
	var mouse_pos: Vector2 = get_viewport().get_mouse_position()
	ghost_tile_node.position = mouse_pos
	ghost_player_node.position = mouse_pos
	ghost_enemy_node.position = mouse_pos
	ghost_orb_node.position = mouse_pos
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
	
	if event.is_action_pressed("editor_edit_mode_1"):
		edit_mode_node.select(0)
		_on_edit_mode_button_item_selected(0)
	
	if event.is_action_pressed("editor_edit_mode_2"):
		edit_mode_node.select(1)
		_on_edit_mode_button_item_selected(1)
	
	if event.is_action_pressed("editor_edit_mode_3"):
		edit_mode_node.select(2)
		_on_edit_mode_button_item_selected(2)
	
	if event.is_action_pressed("editor_edit_mode_4"):
		edit_mode_node.select(3)
		_on_edit_mode_button_item_selected(3)
	
	pass


func _unhandled_input(event: InputEvent) -> void:
	# Detect non-ui mouse inputs
	# Note: Must set this control node to "Mouse > Filter = Pass"
	if event.is_action_pressed("editor_primary_click"):
		var mouse_pos: Vector2 = self.get_global_mouse_position() + camera_node.get_position()
		var edit_mode: int = edit_mode_node.selected
		
		if edit_mode == 0:
			add_tile_button_pressed.emit(mouse_pos)
		elif edit_mode == 1:
			pass # TODO
		elif edit_mode == 2:
			pass # TODO
		elif edit_mode == 3:
			pass # TODO
	
	if event.is_action_pressed("editor_secondary_click"):
		var mouse_pos: Vector2 = self.get_global_mouse_position() + camera_node.get_position()
		var edit_mode: int = edit_mode_node.selected
		
		if edit_mode == 0:
			remove_tile_button_pressed.emit(mouse_pos)
		elif edit_mode == 1:
			pass # TODO
		elif edit_mode == 2:
			pass # TODO
		elif edit_mode == 3:
			pass # TODO
	
	pass


func _on_level_saved(filepath: String) -> void:
	_on_update_thumbnail_button_toggled(filepath)
	pass


func _on_tile_type_changed(atlas_coords: Vector2i) -> void:
	ghost_tile_node.set_tile_type(atlas_coords)
	pass


func _on_tile_alt_changed(tile_alt: int) -> void:
	ghost_tile_node.set_tile_alt(tile_alt)
	pass


func _on_back_button_pressed() -> void:
	SceneManager.get_instance().load_previous_scene(self.get_tree())
	pass


func _on_playtest_button_pressed() -> void:
	playtest_button_pressed.emit()
	pass


func _on_help_button_pressed() -> void:
	help_popup_node.set_visible(true)
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

func _on_test_action_1_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 1)
	pass


func _on_test_action_2_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 2)
	pass


func _on_test_action_3_button_pressed() -> void:
	test_action_button_pressed.emit("Do", 3)
	pass


func _on_show_user_directory_button_pressed() -> void:
	debug_popup_node.set_text("User Data Directory: \n" + OS.get_user_data_dir())
	debug_popup_node.set_visible(true)
	pass


func _on_undo_button_pressed() -> void:
	undo_button_pressed.emit()
	pass


func _on_redo_button_pressed() -> void:
	redo_button_pressed.emit()
	pass


func _on_load_example_level_button_pressed() -> void:
	load_level_popup_node.access = FileDialog.ACCESS_RESOURCES
	load_level_popup_node.set_visible(true)
	pass


func _on_load_level_button_pressed() -> void:
	load_level_popup_node.access = FileDialog.ACCESS_USERDATA
	load_level_popup_node.set_visible(true)
	pass


func _on_load_level_popup_file_selected(path: String) -> void:
	load_level_path_selected.emit(path)
	camera_node.set_position(Vector2(0, 0))
	pass


func _on_save_level_button_pressed() -> void:
	save_level_popup_node.access = FileDialog.ACCESS_USERDATA
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


func _on_reset_level_button_pressed() -> void:
	load_level_path_selected.emit("res://level/level_default_box.tscn")
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
	menu_node.set_visible(!menu_node.is_visible())
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


func _on_grid_button_toggled(toggled_on: bool) -> void:
	grid_layer.visible = toggled_on
	pass

func _on_update_thumbnail_button_toggled(filepath: String) -> void:
	# Set thumbnail filepath
	filepath = filepath.replace(".tscn", ".png").replace("res://", "user://")
	
	# Hide UI
	self.visible = false
	var grid_layer_visible: bool = grid_layer.visible
	grid_layer.visible = false
	
	# Wait for the current frame to finish rendering
	await RenderingServer.frame_post_draw
	
	# Save thumbnail
	var thumbnail: Image = get_viewport().get_texture().get_image()
	thumbnail.save_png(filepath)
	
	# Unhide UI
	self.visible = true
	grid_layer.visible = grid_layer_visible
	
	pass

func _on_edit_mode_button_item_selected(index: int) -> void:
	# Change visibility of edit mode indicators
	ghost_tile_node.visible = false
	ghost_player_node.visible = false
	ghost_enemy_node.visible = false
	ghost_orb_node.visible = false
	
	if index == 0:
		ghost_tile_node.visible = true
	elif index == 1:
		ghost_player_node.visible = true
	elif index == 2:
		ghost_enemy_node.visible = true
	elif index == 3:
		ghost_orb_node.visible = true
	
	pass