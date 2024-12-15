# Author: Jacob Couture
# Description: Controls the animation of the rank text on the results screen

extends Label


@export var max_scale: float
@export var min_scale: float
var time_passed = 0


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	time_passed += delta
	var value
	
	# Calculate new scale value
	value = easeInOutSine(min_scale, max_scale, time_passed, 1) + 1.2
	
	# Reset animation after 2 seconds
	if (time_passed >= 2):
		time_passed = 0
	
	# Apply new scale
	scale.x = value
	scale.y = value


# Custom sine easing function that returns a value clamped between min and max value, and can be controlled with a speed variable
func easeInOutSine(min_val: float, max_val: float, timePassed: float, speed: float):
	return (((max_val - min_val)*(sin(PI * timePassed) * speed + max_val + min_val) / 2))
