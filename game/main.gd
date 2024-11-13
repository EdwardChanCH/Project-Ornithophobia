extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#var scene = load("res://debug.tscn")
	#var instance = scene.instantiate()
	#add_child(instance)
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if Input.is_action_pressed("exit_temp"):
		get_tree().change_scene_to_file("res://main_menu.tscn")
	pass
	
