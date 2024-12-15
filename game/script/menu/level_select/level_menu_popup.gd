# Author: Jacob Couture
# Description: Controller for the level popup on the Main Level Select screen

extends Control


@export var level_path = ""
@export var dir_path = "res://level/story/"
var level_controller_path = "res://level/level_controller.tscn"


# Called when the node is ready in the scene tree.
func _ready() -> void:
	for level in DirAccess.get_files_at(dir_path):
		if (level == level_path):
			load_level(dir_path + level)
			Global.data_index = Global.level_data.find(level)
			break


# Load all metadata from a level to be displayed in this popup
func load_level(level):
	if ResourceLoader.exists(level):
		var level_scene = load(level)
		var level_instance: Level = level_scene.instantiate()
		if level_instance is Level:
			var level_metadata: Dictionary = level_instance.get_level_info()
			%LevelTitle.text = level_metadata.get("level_name", "null")
			var rank_times = level_metadata.get("rank_times", null)
			if (rank_times != null):
				%GoldTime.text = rank_times[0]
				%SilverTime.text = rank_times[1]
				%BronzeTime.text = rank_times[2]
			else:
				%GoldTime.text = "--:--.--"
				%SilverTime.text = "--:--.--"
				%BronzeTime.text = "--:--.--"
			%BestTime.text = level_metadata.get("best_time", "00:00.00")
			%LevelIcon.texture = load(level.replace(".tscn", ".png"))
			%Rank.texture = load(level_metadata.get("rank_icon", "res://asset/sprite/default_texture.png"))
			level_path = level
		level_instance.queue_free()


# Change scenes to the selected level when the play button is pressed
func _on_play_button_pressed() -> void:
	var level_controller: LevelController = SceneManager.get_instance().import_scene_tscn(level_controller_path)
	level_controller.set_level(level_path)
	SceneManager.get_instance().load_new_scene_node(get_tree(), level_controller_path, level_controller)


# Switch to the next level in the level list when the next button is pressed
func _on_next_level_button_pressed() -> void:
	if (Global.data_index == len(Global.level_data) - 1):
		Global.data_index = 0
	else:
		Global.data_index += 1
	load_level(dir_path + Global.level_data[Global.data_index])


# Switch to the previous level in the level list when the last level button is pressed
func _on_last_level_button_pressed() -> void:
	if (Global.data_index == 0):
		Global.data_index = len(Global.level_data) - 1
	else:
		Global.data_index -= 1
	load_level(dir_path + Global.level_data[Global.data_index])


# Control next and last level buttons with A/D or LEFT/RIGHT
func _input(event: InputEvent) -> void:
	if (event.is_action_pressed("ui_right")):
		if (Global.data_index == len(Global.level_data) - 1):
			Global.data_index = 0
		else:
			Global.data_index += 1
		load_level(dir_path + Global.level_data[Global.data_index])
	elif (event.is_action_pressed("ui_left")):
		if (Global.data_index == 0):
			Global.data_index = len(Global.level_data) - 1
		else:
			Global.data_index -= 1
		load_level(dir_path + Global.level_data[Global.data_index])
