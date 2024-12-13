extends PanelContainer


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	visible = Debug.get_singleton().is_debug_mode_active()


func _input(event: InputEvent) -> void:
	if (event.is_action_pressed("debug")):
		visible = !visible
		Debug.get_singleton().set_debug_mode(visible)
