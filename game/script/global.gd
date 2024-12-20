# Author: Jacob Couture
# Description: Auto-load script for helper functions and mock data

extends Node

const MAIN_LEVELS_DIR_PATH = "res://level/story/"
const CUSTOM_LEVELS_DIR_PATH = "user://level/workshop/"
const USER_LEVELS_DIR_PATH = "user://level/user/"

var level_data = []
var data_index = 0


# Creates duplicates of the main 3 levels as Custom Levels for testing
func _ready() -> void:
	create_dir(CUSTOM_LEVELS_DIR_PATH)
	create_dir(USER_LEVELS_DIR_PATH)
	
	#var c_level_1_scene = SceneManager.get_instance().import_scene_tscn("res://level/story/level_001.tscn")
	#var c_level_2_scene = SceneManager.get_instance().import_scene_tscn("res://level/story/level_002.tscn")
	#var c_level_3_scene = SceneManager.get_instance().import_scene_tscn("res://level/story/level_003.tscn")
	#
	#var metadata = c_level_1_scene.get_level_info()
	#metadata["level_author"] = "Jacob"
	#metadata["best_time"] = "00:21.32"
	#Level.export_level_tscn(CUSTOM_LEVELS_DIR_PATH + "level_001.tscn", c_level_1_scene)
	#Level.export_level_tscn(CUSTOM_LEVELS_DIR_PATH + "level_002.tscn", c_level_2_scene)
	#Level.export_level_tscn(CUSTOM_LEVELS_DIR_PATH + "level_003.tscn", c_level_3_scene)
	#c_level_1_scene.queue_free()
	#c_level_2_scene.queue_free()
	#c_level_3_scene.queue_free()


# Helper function to create a new directory at a given filepath
func create_dir(dir):
	if (!DirAccess.dir_exists_absolute(dir)):
		var error_code = DirAccess.make_dir_recursive_absolute(dir)
		if error_code != OK:
			var DIR_ERROR = "ERROR: Failed to create directory '%s'. Error code %s."
			printerr(DIR_ERROR % [dir, error_code])


# Helper function to convert a string representation of time of the format 00:00.00 to a numerical representation
func read_formatted_time(time):
	var units = multi_split(time, [":", "."])
	var total_time = (int(units[0]) * 60) + (int(units[1])) + float(int(units[2])) * 0.01
	return total_time


# Helper function to split a string using multiple delimiters
func multi_split(string, delimiters):
	var tokens = [string]
	
	for d in delimiters:
		var new_tokens = PackedStringArray()
		for t in tokens:
			new_tokens.append_array(t.split(d))
		
		tokens = new_tokens
	
	return tokens
