extends PanelContainer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Global.debug = self
	visible = true


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _input(event):
	if event.is_action_pressed("debug"):
		visible = !visible
