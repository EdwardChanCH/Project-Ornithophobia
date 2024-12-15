extends EnemyController


@export var sfx_player: AudioStreamPlayer


func _on_enemy_died():
	sfx_player.play_hit()
