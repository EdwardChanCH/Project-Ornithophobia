extends ProgressBar

@export var max_slow_time = 2000
@export var regen_rate = 50
@export var soft_max_value = 98

@onready var animation_player: AnimationPlayer = $AnimationPlayer

var cooldown: Timer
var can_fade_in
var can_fade_out
var start_press_time = 0
var can_regenerate = false

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	cooldown = %CooldownTimer
	cooldown.wait_time = 1.0
	value = soft_max_value
	self_modulate.a = 0.0


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# If the meter is full, allow it to fade out. Otherwise, stop the fade out animation
	if (value == soft_max_value):
		can_fade_out = true
		can_fade_in = false
	else:
		can_fade_out = false
		can_fade_in = true
		if (animation_player.is_playing() and animation_player.current_animation == "fade_out"):
			animation_player.stop()
	
	# Play fade out animation if meter is full
	if (value == soft_max_value and self_modulate.a == 1 and can_fade_out):
		animation_player.play("fade_out")

	update_meter()
	
	# If the meter can regenerate, start regenerating
	if (can_regenerate and (value < max_value)):
		value += regen_rate * delta
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
			# Stop all currently playing animations
			if (animation_player.is_playing()):
				animation_player.stop()
				animation_player.clear_queue()
			
			# Set the max slow motion time to be dependent on how full the slow motion meter is
			max_slow_time = value * 20
			
			start_press_time = Time.get_ticks_msec()
			can_regenerate = false
			
		var time_pressed = 0
		if (Input.is_action_pressed("large_blast") and get_parent().can_slow_time()):
			# Stop regen cooldown timer if large blast button is pressed again
			cooldown.stop()
			# Play fade in animation if the slow motion bar isnt visible
			if (value < soft_max_value and self_modulate.a == 0 and can_fade_in):
				#can_fade_in = false
				animation_player.play("fade_in", 1 - self_modulate.a, 1 / Engine.time_scale)
			
			# Update the amount of time the large blast button has been held down for
			time_pressed = Time.get_ticks_msec() - start_press_time
			
			# If the large blast button is held down and there is meter remaining, lower the meter
			if (time_pressed <= max_slow_time):
				var new_value = ((max_slow_time) - time_pressed) / 20
				clamp(new_value, 0, soft_max_value)
				value = new_value
			else:
				value = min_value
				time_pressed = 0
				get_parent().set_can_slow_time(false)
				cooldown.stop()
				cooldown.start(cooldown.wait_time)
	
	# If the large blast button is released or cancelled, start the regen cooldown timer
	if ((Input.is_action_just_released("large_blast") or Input.is_action_just_pressed("small_blast") or get_parent().is_on_floor()) and cooldown.is_stopped() and !can_regenerate and value < soft_max_value):
		cooldown.start(cooldown.wait_time)


# Allow the meter to regenerate when the regen cooldown timer finishes
func _on_cooldown_timer_timeout() -> void:
	can_regenerate = true
