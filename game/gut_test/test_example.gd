extends GutTest

func test_assert_true() -> void:
	# Pretend this result is from a function call
	var result: bool = true
	
	# Format: function_name(actual_outcome, expected_outcome, fail_message)
	assert_eq(result, true, "This should be true.") # Assert Equal
	
	pass # End of function
