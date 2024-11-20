extends Area2D


var levelPopupScene = load("res://scenes/level_menu_popup.tscn")
var levelPopupInstance: Control


func _ready() -> void:
	set_process_input(false)


func _on_mouse_entered() -> void:
	change_border_color(Color.BLUE)


func _on_mouse_exited() -> void:
	change_border_color(Color.BLACK)


func _input_event(_viewport: Viewport, event: InputEvent, _shape_idx: int) -> void:
	if (event is InputEventMouseButton and event.is_action("left_click") and event.is_pressed()):
		set_process_input(true)


func _input(event: InputEvent) -> void:
	if (event is InputEventMouseButton and event.is_action("left_click") and not event.is_pressed()):
		levelPopupInstance = levelPopupScene.instantiate()
		get_tree().root.find_child("LevelSelect", true, false).call_deferred("add_child", levelPopupInstance)
		var file_name = str(%LevelTitle.text).replace(" ", "_")
		file_name = file_name.to_lower() + ".tres"
		Global.cur_level_popup_path = file_name
		set_process_input(false)


func destantiate_popup():
	get_tree().root.find_child("LevelMenuPopup", true, false).queue_free()


func change_border_color(color):
	var hover_stylebox = %Border.get_theme_stylebox("panel").duplicate()
	hover_stylebox.border_color = color
	%Border.add_theme_stylebox_override("panel", hover_stylebox)
