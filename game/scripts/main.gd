extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	print(get_viewport().size.x)
	print(get_viewport().size.y)
	#get_viewport().size.x = 1920
	#get_viewport().size.y = 1056
	
	global_scale.x = 3
	global_scale.y = 3
