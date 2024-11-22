extends ProgressBar

@export var max_slow_time: float = 3000
@export var regen_rate = 50
@export var soft_max_value = 98

@onready var animation_player: AnimationPlayer = $AnimationPlayer

var cooldown: Timer
var can_fade_in
var can_fade_out
var start_press_time = 0
var can_regenerate = false
var can_drain = false
var can_slow_time
var time_slow_factor = max_slow_time / 100

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
	
	if (value > 0):
		can_slow_time = true
	else:
		can_slow_time = false
	
	get_parent().set_can_slow_time(can_drain)
	
	update_meter()
	
	# If the meter can regenerate, start regenerating
	if (can_regenerate and (value < max_value)):
		value += regen_rate * delta
		if (value >= soft_max_value):
			value = soft_max_value
			can_regenerate = false
			
	Debug.get_singleton().add_debug_property("can_drain", can_drain)


func update_meter():
	var time_pressed = 0
	if (Input.is_action_just_pressed("action_button")):
		# Stop all currently playing animations
		if (animation_player.is_playing()):
			animation_player.stop()
			animation_player.clear_queue()
		
		# Set the max slow motion time to be dependent on how full the slow motion meter is
		max_slow_time = value * time_slow_factor
		
		start_press_time = Time.get_ticks_msec()
		can_regenerate = false
		can_drain = true
	
	# If the action button is being held down, slow time
	if (Input.is_action_pressed("action_button") and can_slow_time and Engine.time_scale != 1 and can_drain):
		# Update the amount of time the action button has been held down for
		time_pressed = Time.get_ticks_msec() - start_press_time
		# Stop regen cooldown timer if action button is pressed again
		cooldown.stop()
		# Play fade in animation if the slow motion bar isnt visible
		if (value < soft_max_value and self_modulate.a == 0 and can_fade_in):
			animation_player.play("fade_in", -1, 1 / Engine.time_scale)
		
		# If the action button is held down and there is meter remaining, lower the meter
		if (time_pressed <= max_slow_time):
			var new_value = ((max_slow_time) - time_pressed) / time_slow_factor
			clamp(new_value, 0, soft_max_value)
			value = new_value
		else:
			value = min_value
			time_pressed = 0
			can_slow_time = false
			can_drain = false
			cooldown.stop()
			cooldown.start(cooldown.wait_time)
	
	# If the large blast button is released or cancelled, start the regen cooldown timer
	if ((Input.is_action_just_released("action_button")) and cooldown.is_stopped() and !can_regenerate and value < soft_max_value):
		cooldown.start(cooldown.wait_time)
	Debug.get_singleton().add_debug_property("time_pressed", time_pressed)

# Allow the meter to regenerate when the regen cooldown timer finishes
func _on_cooldown_timer_timeout() -> void:
	can_regenerate = true
