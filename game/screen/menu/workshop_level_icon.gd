extends Control


var level_path = ""
const DIR_PATH = "user://custom_levels/"


func load_level(level):
	if ResourceLoader.exists(level):
		var test_level = ResourceLoader.load(level)
		if test_level is WorkshopLevelData:
			%LevelName.text = test_level.level_name
			%AuthorName.text = test_level.author_name
			%BestTime.text = test_level.best_time
			%LevelIcon.texture = load(test_level.icon_path)
			level_path = test_level.level_path
