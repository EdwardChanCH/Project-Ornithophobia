extends Node

const MAIN_LEVELS_DIR_PATH = "res://level/"
const CUSTOM_LEVELS_DIR_PATH = "user://workshop/custom_levels/"
const USER_LEVELS_DIR_PATH = "user://workshop/user_levels/"

var level_data = []
var data_index = 0

func _ready() -> void:
	create_dir(CUSTOM_LEVELS_DIR_PATH)
	create_dir(USER_LEVELS_DIR_PATH)
	
	var c_level_1_scene = load("res://level/level_001.tscn")
	var c_level_2_scene = load("res://level/level_002.tscn")
	var c_level_3_scene = load("res://level/level_003.tscn")
	
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


#func save_level(level, level_name, rank_times, best_time, icon_path, level_path):
	#if (!DirAccess.dir_exists_absolute(MAIN_LEVELS_DIR_PATH)):
		#var error_code = DirAccess.make_dir_recursive_absolute(MAIN_LEVELS_DIR_PATH)
		#if error_code != OK:
			#var DIR_ERROR = "ERROR: Failed to create directory '%s'. Error code %s."
			#printerr(DIR_ERROR % [MAIN_LEVELS_DIR_PATH, error_code])
	#
	#level.level_name = level_name
	#level.rank_times = rank_times
	#level.best_time = best_time
	#level.icon_path = icon_path
	#level.level_path = level_path
	#
	#if (read_formatted_time(best_time) <= read_formatted_time(rank_times[0])):
		#level.rank_img = "res://assets/sprites/menu/gold_rank_icon.png"
	#elif (read_formatted_time(best_time) <= read_formatted_time(rank_times[1])):
		#level.rank_img = "res://assets/sprites/menu/silver_rank_icon.png"
	#elif (read_formatted_time(best_time) <= read_formatted_time(rank_times[2])):
		#level.rank_img = "res://assets/sprites/menu/bronze_rank_icon.png"
	#else:
		#level.rank_img = "res://assets/sprites/menu/no_rank_icon.png"
#
	#
	#var file_name = str(level_name).replace(" ", "_")
	#file_name = file_name.to_lower() + ".tres"
	#var result = ResourceSaver.save(level, MAIN_LEVELS_DIR_PATH + file_name)
	#assert(result == OK)


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
