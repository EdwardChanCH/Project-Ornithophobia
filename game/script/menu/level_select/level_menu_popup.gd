extends Control


@export var level_path = ""
@export var dir_path = "res://level/story/"


# Called when the node is ready in the scene tree.
func _ready() -> void:
	for level in DirAccess.get_files_at(dir_path):
		if (level == level_path):
			load_level(dir_path + level)
			Global.data_index = Global.level_data.find(level)
			break


func load_level(level):
	if ResourceLoader.exists(level):
		var level_scene = load(level)
		var level_instance: Level = level_scene.instantiate()
		if level_instance is Level:
			var level_metadata: Dictionary = level_instance.get_level_info()
			%LevelTitle.text = level_metadata.get("name", "null")
			var rank_times = level_metadata.get("rank_times", null)
			if (rank_times != null):
				%GoldTime.text = rank_times[0]
				%SilverTime.text = rank_times[1]
				%BronzeTime.text = rank_times[2]
			%BestTime.text = level_metadata.get("best_time", "00:00.00")
			%LevelIcon.texture = load(level_metadata.get("level_icon", "res://asset/sprite/default_texture.png"))
			%Rank.texture = load(level_metadata.get("rank_icon", "res://asset/sprite/default_texture.png"))
			level_path = level
		level_instance.queue_free()
#
#
func _on_play_button_pressed() -> void:
	# get_tree().change_scene_to_file(level_path)
	SceneManager.get_instance().load_new_scene(get_tree(), level_path)


func _on_next_level_button_pressed() -> void:
	if (Global.data_index == len(Global.level_data) - 1):
		Global.data_index = 0
	else:
		Global.data_index += 1
	load_level(dir_path + Global.level_data[Global.data_index])


func _on_last_level_button_pressed() -> void:
	if (Global.data_index == 0):
		Global.data_index = len(Global.level_data) - 1
	else:
		Global.data_index -= 1
	load_level(dir_path + Global.level_data[Global.data_index])


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
