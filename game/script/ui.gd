extends Node


var uiOverlayInstance: Control
var sceneTransitionInstance: CanvasLayer


# Called when the node is ready in the scene tree.
func _ready() -> void:
	uiOverlayInstance = SceneManager.get_instance().import_scene_tscn("res://screen/permanent_ui.tscn")
	get_tree().root.call_deferred("add_child", uiOverlayInstance)


func _exit_tree() -> void:
	uiOverlayInstance.queue_free()
