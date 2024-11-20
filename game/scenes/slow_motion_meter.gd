extends ProgressBar

@export var max_slow_time = 2000
@export var regen_rate = 50
@export var soft_max_value = 98

@onready var animation_player: AnimationPlayer = $AnimationPlayer

var cooldown: Timer
var can_fade_in
var can_fade_out
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	cooldown = %CooldownTimer
	cooldown.wait_time = 1.0
	value = soft_max_value
	self_modulate.a = 0.0
	can_fade_in = true
	can_fade_out = false
	animation_player.animation_finished.connect(_on_animation_finished)


func _on_animation_finished(anim_name):
	if (anim_name == "fade_out"):
		can_fade_in = true
		#self_modulate.a = 0.0
	if (anim_name == "fade_in"):
		can_fade_out = true
		#self_modulate.a = 1.0


var start_press_time = 0
var can_regenerate = false
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if (value == soft_max_value and self_modulate.a == 1 and can_fade_out):
		can_fade_out = false
		can_fade_in = true
		animation_player.play("fade_out")

	update_meter()
	
	if (can_regenerate and (value < max_value)):
		value += regen_rate * delta
		print(value)
		if (value >= soft_max_value):
			value = soft_max_value
			can_regenerate = false
	
	Debug.get_singleton().add_debug_property("slowMeterValue", value)
	Debug.get_singleton().add_debug_property("canRegenerate", can_regenerate)
	Debug.get_singleton().add_debug_property("can_fade_out", can_fade_out)
	Debug.get_singleton().add_debug_property("can_fade_in", can_fade_in)


func update_meter():
	if (!get_parent().is_on_floor()):
		if (Input.is_action_just_pressed("large_blast")):
			if (animation_player.is_playing()):
				animation_player.stop()
				animation_player.clear_queue()
			max_slow_time = value * 20
			start_press_time = Time.get_ticks_msec()
			can_regenerate = false
			
		var time_pressed = 0
		if (Input.is_action_pressed("large_blast") and get_parent().can_slow_time()):
			if (value < soft_max_value and self_modulate.a == 0 and can_fade_in):
				self_modulate.a = 0.0
				can_fade_in = false
				animation_player.play("fade_in", 1 - self_modulate.a, 1 / Engine.time_scale)
			time_pressed = Time.get_ticks_msec() - start_press_time
			if (time_pressed <= max_slow_time):
				var temp = ((max_slow_time) - time_pressed) / 20
				if (temp > soft_max_value):
					temp = soft_max_value
				value = temp
			else:
				value = min_value
				time_pressed = 0
				get_parent().set_can_slow_time(false)
				cooldown.start()

		if (Input.is_action_just_released("large_blast") or Input.is_action_just_pressed("small_blast")):
			cooldown.start()


func _on_cooldown_timer_timeout() -> void:
	can_regenerate = true
