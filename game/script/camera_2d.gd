extends Camera2D

@export var random_strength = 30.0
@export var shake_fade = 5.0

var rng = RandomNumberGenerator.new()
var shake_strength = 0.0
@onready var player: PlayerController = get_parent().find_child("PlayerList").get_children().front()


func apply_shake():
	shake_strength = random_strength * (player.velocity.length() / 1500)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if (shake_strength > 0):
		shake_strength = lerpf(shake_strength, 0, shake_fade * delta)
		offset = random_offset()


func random_offset() -> Vector2:
	return Vector2(rng.randf_range(-shake_strength, shake_strength) + 960, rng.randf_range(-shake_strength, shake_strength) + 540)
