# Author: Jacob Couture
# Description: Controller for the pause menu

extends Control


# Unpauses when the resume button is pressed
func _on_resume_button_pressed() -> void:
	toggle_pause()


# Unpause and change to the previous scene when the return to menu button is pressed
func _on_return_to_menu_button_pressed() -> void:
	get_tree().paused = false
	visible = false
	SceneManager.get_instance().load_previous_scene(get_tree())


# Exit the game when the exit to desktop button is pressed
func _on_exit_button_pressed() -> void:
	get_tree().paused = false
	get_tree().quit()


# Toggle pause when the escape button is pressed
func _input(event: InputEvent) -> void:
	if (event.is_action_pressed("escape")):
		toggle_pause()


# Toggles the game pause state and set this menu to be visible. Update mouse mode to no longer be locked to the screen while this menu is open
func toggle_pause():
	get_tree().paused = !get_tree().paused
	if (get_tree().paused):
		visible = true
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		visible = false
		Input.mouse_mode = Input.MOUSE_MODE_CONFINED
