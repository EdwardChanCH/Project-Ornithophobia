extends Control

const DIR_PATH = "res://level/"
const LEVEL_NAME_PATTERN_RE = "level_[0-9]{3}[.]tscn"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var regex = RegEx.new()
	regex.compile(LEVEL_NAME_PATTERN_RE)
	
	for level_path in DirAccess.get_files_at(DIR_PATH):
		if (regex.search(level_path)):
			var levelIconScene = load("res://screen/menu/level_icon.tscn")
			var levelIconInstance: Control = levelIconScene.instantiate()
			%LevelContainer.call_deferred("add_child", levelIconInstance)
			
			var level_scene = load(DIR_PATH + level_path)
			var level_instance: Level = level_scene.instantiate()
			var level_metadata: Dictionary = level_instance.get_level_info()
			levelIconInstance.find_child("LevelTitle").text = level_metadata.get("name", "null")
			levelIconInstance.find_child("LevelIcon").texture = load(level_metadata.get("level_icon", "res://asset/sprite/default_texture.png"))
			levelIconInstance.find_child("Rank").texture = load(level_metadata.get("rank_icon", "res://asset/sprite/default_texture.png"))
			levelIconInstance.level_path = level_path
			Global.level_data.append(level_path)
			
			level_instance.queue_free()


func _exit_tree() -> void:
	for child in %LevelContainer.get_children():
		child.queue_free()
