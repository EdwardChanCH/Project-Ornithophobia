extends EnemyController

signal hitstop_end

func _on_enemy_died():
	var level: LevelController = get_parent().get_parent().find_child("LevelController")
	hitstop(0.2)


func hitstop(duration):
	if (Engine.time_scale == 1):
		get_tree().paused = true
		$SFXPlayer.play_hit()
		await(get_tree().create_timer(duration, true, false, true).timeout)
		get_tree().paused = false
		emit_signal("hitstop_end")
