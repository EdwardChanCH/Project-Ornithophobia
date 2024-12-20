# Author: Sameer Bugti
# Description: Audio player for player sound effects

extends AudioStreamPlayer


static var left = preload("res://asset/audio/leftSFX.mp3")
static var right = preload("res://asset/audio/rightSFX.mp3")


func _on_left_click() -> void:
	play_left()


func _on_right_click() -> void:
	play_right()


func _process(_delta: float) -> void:
	pitch_scale = Engine.time_scale
	

func play_left() -> void:
	stream = left
	play()


func play_right() -> void:
	stream = right
	play()
