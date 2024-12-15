# Author: Jacob Couture
# Description: Makes a button change scenes to the previous scene upon being pressed

extends Button


func _on_pressed() -> void:
	SceneManager.get_instance().load_previous_scene(get_tree())
	pass
