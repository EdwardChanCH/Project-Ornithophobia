# Author: Jacob Couture
# Description: Controller for the Workshop Level Select screen

extends Control

@export var current_tab = "custom"
@export var level_name_pattern_re = ".*quicksave[.]tscn"
var regex


# Called when the node is ready in the scene tree.
func _ready() -> void:
	get_tree().paused = false
	regex = RegEx.new()
	regex.compile(level_name_pattern_re)
	reload_levels(Global.CUSTOM_LEVELS_DIR_PATH)


# Reload the levels on the Workshop Level Select screen
func reload_levels(dir_path):
	free_children()
	var scroll_buffer: ColorRect = ColorRect.new()
	scroll_buffer.color = Color.TRANSPARENT
	scroll_buffer.custom_minimum_size.y = 16
	%LevelContainer.call_deferred("add_child", scroll_buffer)
	var num_children = 0
	for level_path in DirAccess.get_files_at(dir_path):
		if (regex.search(level_path) or level_path.ends_with(".png")):
			continue
		
		var levelBannerScene = load("res://screen/menu/workshop_level_icon.tscn")
		var levelBannerInstance: Control = levelBannerScene.instantiate()
		%LevelContainer.call_deferred("add_child", levelBannerInstance)
		
		# Set level banner level data
		var level_instance: Level = SceneManager.get_instance().import_scene_tscn(dir_path + level_path)
		var level_metadata: Dictionary = level_instance.get_level_info()
		levelBannerInstance.find_child("LevelName", true, false).text = level_metadata.get("level_name", "null")
		var img = Image.new()
		img.load(dir_path + level_path.replace(".tscn", ".png"))
		levelBannerInstance.find_child("LevelIcon", true, false).texture = ImageTexture.create_from_image(img)
		levelBannerInstance.find_child("AuthorName", true, false).text = level_metadata.get("level_author", "null")
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
	if (current_tab == "custom"):
		OS.shell_show_in_file_manager(ProjectSettings.globalize_path(Global.CUSTOM_LEVELS_DIR_PATH))
	elif (current_tab == "user"):
		OS.shell_show_in_file_manager(ProjectSettings.globalize_path(Global.USER_LEVELS_DIR_PATH))


func _on_refresh_levels_button_pressed() -> void:
	if (current_tab == "custom"):
		reload_levels(Global.CUSTOM_LEVELS_DIR_PATH)
	elif (current_tab == "user"):
		reload_levels(Global.USER_LEVELS_DIR_PATH)


func _on_create_button_pressed() -> void:
	SceneManager.get_instance().load_new_scene(get_tree(), "res://screen/level_editor.tscn")
	pass
