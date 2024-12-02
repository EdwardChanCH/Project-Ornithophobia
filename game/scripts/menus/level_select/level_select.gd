extends Control


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for level_path in DirAccess.get_files_at("user://levels/"):
		var levelIconScene = load("res://screen/menu/level_icon.tscn")
		var levelIconInstance: Control = levelIconScene.instantiate()
		%LevelContainer.call_deferred("add_child", levelIconInstance)
		var test_level = ResourceLoader.load("user://levels/" + level_path)
		levelIconInstance.find_child("LevelTitle").text = test_level.level_name
		levelIconInstance.find_child("LevelIcon").texture = load(test_level.icon_path)
		levelIconInstance.find_child("Rank").texture = load(test_level.rank_img)
		Global.level_data.append(level_path)


func _exit_tree() -> void:
	for child in %LevelContainer.get_children():
		child.queue_free()
