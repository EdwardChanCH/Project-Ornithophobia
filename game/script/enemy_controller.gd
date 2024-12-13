extends EnemyController


func _on_enemy_died():
	$SFXPlayer.play_hit()
