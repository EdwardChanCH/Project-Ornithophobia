extends Node


var uiOverlayScene = load("res://screen/permanent_ui.tscn")
var uiOverlayInstance
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	uiOverlayInstance = uiOverlayScene.instantiate()
	get_tree().root.call_deferred("add_child", uiOverlayInstance)


func _exit_tree() -> void:
	get_tree().root.find_child("PermanentUI", true, false).queue_free()
