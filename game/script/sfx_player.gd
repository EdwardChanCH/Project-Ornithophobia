# Author: Jacob Couture
# Description: Audio player for enemy sound effects.

extends AudioStreamPlayer


static var hit = preload("res://asset/audio/hitHurt2.wav")
static var death = preload("res://asset/audio/enemy_death.mp3")


func _on_enemy_died() -> void:
	play_death()


func _on_bounce_player(_enemy_pos: Vector2) -> void:
	play_hit()


func _process(_delta: float) -> void:
	pitch_scale = Engine.time_scale
	

func play_death() -> void:
	stream = death
	play()


func play_hit() -> void:
	stream = hit
	play()
