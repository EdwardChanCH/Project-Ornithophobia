extends GutTest


func test_level_editor_controller_constructor() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	
	assert_not_null(editor)
	
	pass # End of function


func test_level_editor_controller_track_action() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()

	editor.track_action(Callable(), Callable())
	assert_true(editor.can_undo())
	
	pass # End of function


func test_level_editor_controller_undo_action() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	var stub_level: Level = partial_double(Level).new()
	var level_info: Dictionary = Dictionary()
	level_info["key1"] = "value1"
	
	stub(stub_level.get_level_info).to_return(level_info) # Same as EXPECT_CALL() in GMock
	editor.track_action(Callable(), Callable(stub_level, "get_level_info"))
	assert_eq_deep(level_info, editor.undo_action()) 
	
	pass # End of function


func test_level_editor_controller_redo_action() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	var stub_level: Level = partial_double(Level).new()
	var level_info: Dictionary = Dictionary()
	level_info["key1"] = "value1"
	
	stub(stub_level.get_level_info).to_return(level_info) # Same as EXPECT_CALL() in GMock
	editor.track_action(Callable(stub_level, "get_level_info"), Callable())
	editor.undo_action()
	assert_eq_deep(level_info, editor.redo_action())
	
	pass # End of function


func test_level_editor_controller_clear_action() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	
	editor.track_action(Callable(), Callable())
	assert_true(editor.can_undo())
	editor.clear_action()
	assert_false(editor.can_undo())
	
	pass # End of function


func test_level_editor_controller_can_undo() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	
	assert_false(editor.can_undo())
	editor.track_action(Callable(), Callable())
	assert_true(editor.can_undo())
	editor.undo_action()
	assert_false(editor.can_undo())

	pass # End of function


func test_level_editor_controller_can_redo() -> void:
	var editor: LevelEditorController = partial_double(LevelEditorController).new()
	
	assert_false(editor.can_redo())
	editor.track_action(Callable(), Callable())
	assert_false(editor.can_redo())
	editor.undo_action()
	assert_true(editor.can_redo())
	
	pass # End of function


# The remainder of LevelEditorController's functions are simply redirected calls to Level, 
# so they are tested in test_level.gd instead.
