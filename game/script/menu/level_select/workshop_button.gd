@tool
extends Control

@export var button_icon: Texture
@export var button_text: String

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	$ButtonIcon.texture = button_icon
	$ButtonText.text = button_text
