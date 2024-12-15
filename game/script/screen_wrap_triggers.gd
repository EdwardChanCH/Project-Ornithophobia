extends Node2D


@onready var player: PlayerController = %PlayerController
@onready var left_exit = $LeftExit
@onready var right_exit = $RightExit
@onready var top_exit = $TopExit
@onready var bottom_exit = $BottomExit


func _on_left_entrance_body_entered(body: Node2D) -> void:
	if (body.name == "PlayerController"):
		player.global_position.x = right_exit.global_position.x


func _on_right_entrance_body_entered(body: Node2D) -> void:
	if (body.name == "PlayerController"):
		player.global_position.x = left_exit.global_position.x


func _on_top_entrance_body_entered(body: Node2D) -> void:
	if (body.name == "PlayerController"):
		player.global_position.y = bottom_exit.global_position.y


func _on_bottom_entrance_body_entered(body: Node2D) -> void:
	if (body.name == "PlayerController"):
		player.global_position.y = top_exit.global_position.y
