extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_open_edward_main_button_pressed() -> void:
	print("Changing scene to EdwardMain...")
	get_tree().change_scene_to_file("res://edward_main.tscn")
