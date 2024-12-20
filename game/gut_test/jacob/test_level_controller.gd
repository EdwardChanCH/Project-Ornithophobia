extends GutTest


var level_controller_scene = load("res://level/level_controller.tscn")
var doubled_level_controller: LevelController


func before_all():
	set_double_strategy(DOUBLE_STRATEGY.INCLUDE_NATIVE)


func test_level_controller_set_empty_level():
	var test_level_path = "res://level/level_default_box.tscn"
	doubled_level_controller = partial_double(level_controller_scene).instantiate()
	
	# Initialize the level controller's level
	doubled_level_controller.set_level(test_level_path)
	
	# Ensure the level actually exists
	assert_not_null(doubled_level_controller.get_level(), "Level node is null. Check if a valid level path has been given.")
	
	var level_info = doubled_level_controller.get_level().get_level_info()
	
	# Ensure all of the level attributes were set up with default values instead of being left null
	for attribute in level_info:
		assert_not_null(level_info[attribute], "Attribute " + str(attribute) + " is null.")


func test_level_controller_set_invalid_level():
	var test_level_path = "res://level/not_a_valid_level.tscn"
	doubled_level_controller = partial_double(level_controller_scene).instantiate()
	
	# Initialize the level controller's level
	doubled_level_controller.set_level(test_level_path)
	
	# Ensure the level controller properly handles an invalid filepath
	assert_null(doubled_level_controller.get_level(), "LevelController loaded a file that doesn't exist.")


func test_level_controller_results_sequence():
	var test_level_path = "res://level/level_default_box.tscn"
	doubled_level_controller = partial_double(level_controller_scene).instantiate()

	# Begin watching signal emissions from the level controller node
	watch_signals(doubled_level_controller)
	
	# Initialize the level
	doubled_level_controller.set_level(test_level_path)
	
	# Mimic all enemies being defeated
	for obj in doubled_level_controller.get_level().get_list("EnemyList").get_children():
		doubled_level_controller._update_enemy_count()
	
	# Ensure the results sequence begins after defeating all enemies
	assert_signal_emitted(doubled_level_controller, "results_slow_time")


# --------------------------------------------------------------------------- #

# This testing function is broken due to GDExtension. The purpose of this test
# case is to emulate user input for reloading a scene with debug mode active,
# however the overriden _input function inside GDExtension classes cannot be
# accessed by GUT. Modifying this function to test a GDScript file that has an
# _input function would show that it works as intended.

#func test_level_controller_debug_reload_scene():
	#var test_level_path = "res://level/level_default_box.tscn"
	#doubled_level_controller = partial_double(LevelController).new()
	#var input_sender = InputSender.new(doubled_level_controller)
	#
	#doubled_level_controller.set_level(test_level_path)
	## Set debug mode to be active before sending inputs
	#Debug.get_singleton().set_debug_mode(true)
	#
	## Emulate the reload scene button being pressed
	#input_sender.action_down("debug_reload_scene")
	#
	## Expect the _input function to call the correct function to reload the level
	#stub(doubled_level_controller, "_input")\
	#.when_passed(InputFactory.action_down("debug_reload_scene"))\
	#.to_call(doubled_level_controller._on_retry_button_pressed)
	#
	## Ensure the correct function was called
	#assert_called(doubled_level_controller, "_on_retry_button_pressed")

# --------------------------------------------------------------------------- #


func test_level_controller_read_formatted_time():
	doubled_level_controller = partial_double(level_controller_scene).instantiate()
	var test_time = "99:99.99"
	
	# Ensure that the C++ implementation of read_formatted_time works the same as the GDScript implementation (with a precision error of 0.005)
	assert_almost_eq(doubled_level_controller.read_formatted_time(test_time), Global.read_formatted_time(test_time), 0.005)
