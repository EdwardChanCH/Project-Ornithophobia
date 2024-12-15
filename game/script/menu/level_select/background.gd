# Author: Jacob Couture
# Description: Makes the Main Level Select level popup disappear when clicking the darkened background

extends Panel


func _gui_input(event: InputEvent) -> void:
	if (event.is_action_pressed("left_click")):
		get_tree().call_group("level_select", "destantiate_popup")
	pass
