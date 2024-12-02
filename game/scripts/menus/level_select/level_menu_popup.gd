extends Control


var level_path = ""
const DIR_PATH = "user://levels/"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for level in DirAccess.get_files_at("user://levels/"):
		if (level == Global.cur_level_popup_path):
			load_level(DIR_PATH + level)
			Global.data_index = Global.level_data.find(level)
			break


func load_level(level):
	if ResourceLoader.exists(level):
		var test_level = ResourceLoader.load(level)
		if test_level is TestIO:
			%LevelTitle.text = test_level.level_name
			%GoldTime.text = test_level.rank_times[0]
			%SilverTime.text = test_level.rank_times[1]
			%BronzeTime.text = test_level.rank_times[2]
			%BestTime.text = test_level.best_time
			%LevelIcon.texture = load(test_level.icon_path)
			%Rank.texture = load(test_level.rank_img)
			level_path = test_level.level_path
#
#
func _on_play_button_pressed() -> void:
	get_tree().change_scene_to_file(level_path)


func _on_next_level_button_pressed() -> void:
	if (Global.data_index == len(Global.level_data) - 1):
		Global.data_index = 0
	else:
		Global.data_index += 1
	load_level(DIR_PATH + Global.level_data[Global.data_index])


func _on_last_level_button_pressed() -> void:
	if (Global.data_index == 0):
		Global.data_index = len(Global.level_data) - 1
	else:
		Global.data_index -= 1
	load_level(DIR_PATH + Global.level_data[Global.data_index])


func _input(event: InputEvent) -> void:
	if (event.is_action_pressed("ui_right")):
		if (Global.data_index == len(Global.level_data) - 1):
			Global.data_index = 0
		else:
			Global.data_index += 1
		load_level(DIR_PATH + Global.level_data[Global.data_index])
	elif (event.is_action_pressed("ui_left")):
		if (Global.data_index == 0):
			Global.data_index = len(Global.level_data) - 1
		else:
			Global.data_index -= 1
		load_level(DIR_PATH + Global.level_data[Global.data_index])
