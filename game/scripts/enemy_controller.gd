extends EnemyController

@onready var death_anim = $DeathAnimation

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	death_anim.stop()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
