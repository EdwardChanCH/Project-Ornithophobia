# Author: Jacob Couture
# Description: Custom button for the My Levels tab

extends Panel


signal activating


var stylebox: StyleBoxFlat
var is_active = false
var offset = 20
var active_stylebox = Color("ffffff")
var inactive_stylebox = Color("c8c8c8")
var hover_color = Color("d8d8d8")


# Called when the node is ready in the scene tree.
func _ready() -> void:
	stylebox = get_theme_stylebox("panel")
	stylebox.bg_color = inactive_stylebox


# Set this tab to be active when clicked
func _on_my_levels_collision_mouse_clicked() -> void:
	if (!is_active):
		is_active = true
		activating.emit()
		stylebox.bg_color = active_stylebox
		position.y -= offset
		z_index = 1


# Deactivate this tab when the Custom Levels tab is activated
func _on_custom_levels_tab_activating() -> void:
	is_active = false
	position.y += offset
	stylebox.bg_color = inactive_stylebox
	z_index = 0


func _on_mouse_entered() -> void:
	if (!is_active):
		stylebox.bg_color = hover_color


func _on_mouse_exited() -> void:
	if (!is_active):
		stylebox.bg_color = inactive_stylebox
