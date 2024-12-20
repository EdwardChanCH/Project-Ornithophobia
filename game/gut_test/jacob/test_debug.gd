extends GutTest


func test_debug_get_singleton():
	var result: Debug = Debug.get_singleton()
	
	assert_eq(result.get_instance_id(), Debug.get_singleton().get_instance_id(), "Debug is a singleton that should have a single instance.")


func test_debug_add_property():
	# Add new property to the list of tracked properties
	var test_property_key = "test_property"
	var test_property_value = 100
	Debug.get_singleton().add_debug_property(test_property_key, test_property_value)
	
	# Ensure property was successfully added
	assert_eq(Debug.get_singleton().get_debug_properties().get(test_property_key), test_property_value)
	
	# Update property
	var test_property_new_value = 200
	Debug.get_singleton().add_debug_property(test_property_key, test_property_new_value)
	
	# Ensure property was successfully updated
	assert_eq(Debug.get_singleton().get_debug_properties().get(test_property_key),test_property_new_value)
	assert_eq(Debug.get_singleton().get_debug_properties().size(), 1)
