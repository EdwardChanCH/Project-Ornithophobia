extends Control

var current_tab = "custom"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	reload_levels(Global.CUSTOM_LEVELS_DIR_PATH)

func reload_levels(dir_path):
	free_children()
	var scroll_buffer: ColorRect = ColorRect.new()
	scroll_buffer.color = Color.TRANSPARENT
	scroll_buffer.custom_minimum_size.y = 16
	%LevelContainer.call_deferred("add_child", scroll_buffer)
	var num_children = 0
	for level_path in DirAccess.get_files_at(dir_path):
		var levelBannerScene = load("res://screen/menu/workshop_level_icon.tscn")
		var levelBannerInstance: Control = levelBannerScene.instantiate()
		%LevelContainer.call_deferred("add_child", levelBannerInstance)
		
		var level_scene = load(dir_path + level_path)
		var level_instance: Level = level_scene.instantiate()
		var level_metadata: Dictionary = level_instance.get_level_info()
		levelBannerInstance.find_child("LevelName", true, false).text = level_metadata.get("name", "null")
		levelBannerInstance.find_child("LevelIcon", true, false).texture = load(level_metadata.get("level_icon", "res://assets/sprites/default_texture.png"))
		levelBannerInstance.find_child("AuthorName", true, false).text = level_metadata.get("author", "null")
		levelBannerInstance.find_child("BestTime", true, false).text = level_metadata.get("best_time", "00:00.00")
		levelBannerInstance.level_path = dir_path + level_path
		level_instance.queue_free()
		num_children += 1
	%LevelContainer.call_deferred("add_child", scroll_buffer.duplicate())
	
	if (num_children > 0):
		$NoLevelsFound.visible = false
	else:
		$NoLevelsFound.visible = true

func _exit_tree() -> void:
	free_children()

func _on_custom_levels_tab_activating() -> void:
	reload_levels(Global.CUSTOM_LEVELS_DIR_PATH)
	current_tab = "custom"

func _on_my_levels_tab_activating() -> void:
	reload_levels(Global.USER_LEVELS_DIR_PATH)
	current_tab = "user"

func free_children():
	for child in %LevelContainer.get_children():
		child.queue_free()

func _on_level_folder_button_pressed() -> void:
	OS.shell_show_in_file_manager(ProjectSettings.globalize_path(Global.CUSTOM_LEVELS_DIR_PATH))

func _on_refresh_levels_button_pressed() -> void:
	if (current_tab == "custom"):
		reload_levels(Global.CUSTOM_LEVELS_DIR_PATH)
	elif (current_tab == "user"):
		reload_levels(Global.USER_LEVELS_DIR_PATH)


func _on_create_button_pressed() -> void:
	# Create new level editor instance here
	pass # Replace with function body.
