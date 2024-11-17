extends Node2D

var editor: LevelEditorController = LevelEditorController.new()


func test_print(s: String) -> void:
	print("Print: ")
	print(s)


func test_unprint(s: String) -> void:
	print("Unprint: ")
	print(s)


func _on_back_button_pressed() -> void:
	print("Changing scene to EdwardMain...")
	SceneManager.get_instance().load_previous_scene(get_tree())
	# get_tree().change_scene_to_file("res://edward_main.tscn")


func _on_debug_scene_manager_pressed() -> void:
	SceneManager.get_instance()._debug()


func _on_test_add_scene_pressed() -> void:
	print("Adding scene to stack...")
	SceneManager.get_instance().load_new_scene(get_tree(), "res://level_editor_ui.tscn")


func _on_test_action_1_pressed() -> void:
	var call1: Callable = Callable(self, "test_print").bind(String("do action 1"))
	var call2: Callable = Callable(self, "test_unprint").bind(String("undo action 1"))
	editor.do_action(call1, call2)


func _on_test_action_2_pressed() -> void:
	var call1: Callable = Callable(self, "test_print").bind(String("do action 2"))
	var call2: Callable = Callable(self, "test_unprint").bind(String("undo action 2"))
	editor.do_action(call1, call2)


func _on_test_action_3_pressed() -> void:
	var call1: Callable = Callable(self, "test_print").bind(String("do action 3"))
	var call2: Callable = Callable(self, "test_unprint").bind(String("undo action 3"))
	editor.do_action(call1, call2)


func _on_undo_button_pressed() -> void:
	editor.undo_action()


func _on_redo_button_pressed() -> void:
	editor.redo_action()


func _on_debug_level_editor_controller_pressed() -> void:
	editor._debug()
