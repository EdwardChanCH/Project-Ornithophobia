extends Label


@export var max_scale: float
@export var min_scale: float
var time_passed = 0


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	time_passed += delta
	var new_scale = scale
	var value
	
	value = easeInOutSine(min_scale, max_scale, time_passed, 1) + 1
	
	if (time_passed >= 2):
		time_passed = 0
	
	new_scale.x = value
	new_scale.y = value
	
	scale = new_scale


func easeInOutSine(min_val: float, max_val: float, timePassed: float, speed: float):
	return (((max_val - min_val)*(sin(PI * timePassed) * speed + max_val + min_val) / 2))
