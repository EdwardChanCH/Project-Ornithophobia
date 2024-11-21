extends Node

const DIR_PATH = "user://levels/"

var cur_level_popup_path = ""
var temp_level_data = [
	["Test Level 1", ["00:30.00", "00:45.00", "01:00.00"], "00:32.87", "res://assets/sprites/menu/test/test_level_icon.png", "res://scenes/main.tscn"],
	["Test Level 2", ["00:23.00", "00:27.00", "00:30.00"], "00:16.31", "res://assets/sprites/menu/test/test_level_icon.png", "res://scenes/main.tscn"],
	["Test Level 3", ["00:51.00", "01:15.00", "01:46.00"], "01:45.42", "res://assets/sprites/menu/test/test_level_icon.png", "res://scenes/main.tscn"]
]
var level_data = []
var data_index = 0

func _ready() -> void:
	# Register levels
	for data in temp_level_data:
		save_level(TestIO.new(), data[0], data[1], data[2], data[3], data[4])
		print("wee")


func save_level(level, level_name, rank_times, best_time, icon_path, level_path):
	if (!DirAccess.dir_exists_absolute(DIR_PATH)):
		var error_code = DirAccess.make_dir_recursive_absolute(DIR_PATH)
		if error_code != OK:
			var DIR_ERROR = "ERROR: Failed to create directory '%s'. Error code %s."
			printerr(DIR_ERROR % [DIR_PATH, error_code])
	
	level.level_name = level_name
	level.rank_times = rank_times
	level.best_time = best_time
	level.icon_path = icon_path
	level.level_path = level_path
	
	if (read_formatted_time(best_time) <= read_formatted_time(rank_times[0])):
		level.rank_img = "res://assets/sprites/menu/gold_rank_icon.png"
	elif (read_formatted_time(best_time) <= read_formatted_time(rank_times[1])):
		level.rank_img = "res://assets/sprites/menu/silver_rank_icon.png"
	elif (read_formatted_time(best_time) <= read_formatted_time(rank_times[2])):
		level.rank_img = "res://assets/sprites/menu/bronze_rank_icon.png"
	else:
		level.rank_img = "res://assets/sprites/menu/no_rank_icon.png"

	
	var file_name = str(level_name).replace(" ", "_")
	file_name = file_name.to_lower() + ".tres"
	var result = ResourceSaver.save(level, DIR_PATH + file_name)
	assert(result == OK)


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
