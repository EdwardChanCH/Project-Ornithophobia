extends EnemyController

signal hitstop_end

func _on_enemy_died():
	
	#var camera: Camera2D = get_parent().get_parent().find_child("PlayerController")
	#var hitstop_length = 2
	#if (player.velocity.length() >= 2000):
		#hitstop_length = 4
	#elif (player.velocity.length() >= 1500):
		#hitstop_length = 3
	#else:
		#hitstop_length = 2
	$SFXPlayer.play_hit()


#func hitstop(duration):
	#if (Engine.time_scale == 1):
		#get_tree().paused = true
		#$SFXPlayer.play_hit()
		#await(get_tree().create_timer(duration, true, false, true).timeout)
		#get_tree().paused = false
		#emit_signal("hitstop_end")
