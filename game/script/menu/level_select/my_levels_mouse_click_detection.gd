# Author: Jacob Couture
# Description: Custom mouse click detection using an Area2D node

extends Area2D


signal mouse_clicked
var mouse_in_area = false


func _process(_delta: float) -> void:
	if (Input.is_action_just_pressed("left_click") and mouse_in_area):
		mouse_clicked.emit()


func _on_mouse_entered() -> void:
	mouse_in_area = true


func _on_mouse_exited() -> void:
	mouse_in_area = false
