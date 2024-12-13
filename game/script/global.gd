extends Node

const MAIN_LEVELS_DIR_PATH = "res://level/story/"
const CUSTOM_LEVELS_DIR_PATH = "user://workshop/custom_levels/"
const USER_LEVELS_DIR_PATH = "user://workshop/user_levels/"

var level_data = []
var data_index = 0

func _ready() -> void:
	create_dir(CUSTOM_LEVELS_DIR_PATH)
	create_dir(USER_LEVELS_DIR_PATH)
	
	var c_level_1_scene = load("res://level/story/level_001.tscn")
	var c_level_2_scene = load("res://level/story/level_002.tscn")
	var c_level_3_scene = load("res://level/story/level_003.tscn")
	
	ResourceSaver.save(c_level_1_scene, CUSTOM_LEVELS_DIR_PATH + "level_001.tscn")
	ResourceSaver.save(c_level_2_scene, CUSTOM_LEVELS_DIR_PATH + "level_002.tscn")
	ResourceSaver.save(c_level_3_scene, CUSTOM_LEVELS_DIR_PATH + "level_003.tscn")
	pass


func create_dir(dir):
	if (!DirAccess.dir_exists_absolute(dir)):
		var error_code = DirAccess.make_dir_recursive_absolute(dir)
		if error_code != OK:
			var DIR_ERROR = "ERROR: Failed to create directory '%s'. Error code %s."
			printerr(DIR_ERROR % [dir, error_code])


func read_formatted_time(time):
	var units = multi_split(time, [":", "."])
	var total_time = (int(units[0]) * 60) + (int(units[1])) + float(int(units[2])) * 0.01
	return total_time

func multi_split(string, delimiters):
	var tokens = [string]
	
	for d in delimiters:
		var new_tokens = PackedStringArray()
		for t in tokens:
			new_tokens.append_array(t.split(d))
		
		tokens = new_tokens
	
	return tokens


func convert_to_file_name(string):
	var file_name = string.replace(" ", "_")
	return file_name.to_lower() + ".tres"
