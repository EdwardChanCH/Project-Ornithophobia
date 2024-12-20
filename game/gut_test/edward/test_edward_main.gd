extends GutTest


func test_edward_main_constructor() -> void:
	var edward_main: EdwardMain = partial_double(EdwardMain).new()
	
	assert_not_null(edward_main)
	
	pass # End of function
