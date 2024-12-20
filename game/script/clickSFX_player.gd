# Author: Sameer Bugti
# Description: Audio player for player sound effects

extends AudioStreamPlayer


static var left = preload("res://asset/audio/leftSFX.mp3")
static var right = preload("res://asset/audio/rightSFX.mp3")


func _input(event: InputEvent) -> void:
	if (event.is_action_pressed("small_blast")):
		play_left()
	elif (event.is_action_pressed("large_blast")):
		play_right()


func _process(_delta: float) -> void:
	pitch_scale = Engine.time_scale
	

func play_left() -> void:
	stream = left
	play()


func play_right() -> void:
	stream = right
	play()
