extends AudioStreamPlayer


func _on_enemy_controller_enemy_died() -> void:
	play()


func _process(_delta: float) -> void:
	pitch_scale = Engine.time_scale
