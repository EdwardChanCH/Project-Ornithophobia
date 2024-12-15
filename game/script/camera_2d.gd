# Author: Jacob Couture
# Description: Makes the camera shake.

extends Camera2D

# Base strength for the camera shake
@export var base_strength = 30.0
# Decay rate of the camera shake
@export var shake_fade = 5.0

# Random number generator for generating a random amount of strength
var rng = RandomNumberGenerator.new()
# Current strength of the camera shake
var shake_strength = 0.0
# Reference to the player node
@onready var player: PlayerController = get_parent().find_child("PlayerList", true, false).get_children().front()


# Applies camera shake, with its intensity being based on the player's current velocity
func apply_shake():
	shake_strength = base_strength * (player.velocity.length() / 1500) * Engine.time_scale


# Called every frame. 'delta' is the elapsed time since the previous frame
func _process(delta: float) -> void:
	if (shake_strength > 0):
		shake_strength = lerpf(shake_strength, 0, shake_fade * delta)
		offset = random_offset()


# Returns a random offset for the camera shake so it feels different every time
func random_offset() -> Vector2:
	return Vector2(rng.randf_range(-shake_strength, shake_strength) + 960, rng.randf_range(-shake_strength, shake_strength) + 540)
