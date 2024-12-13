extends Control


# Called when the node is ready in the scene tree.
func _ready() -> void:
	hide_elements()


func _on_results_screen_show(level_metadata: Dictionary):
	hide_elements()
	print(level_metadata)
	visible = true
	$AnimationPlayer.play("fade_in")
	await(get_tree().create_timer(0.8).timeout)
	%Rank.visible = true
	show_result_attribute(%RankLabel)
	show_result_attribute(%RankFlavourText)


func show_result_attribute(node: Control):
	node.visible = true
	$AudioStreamPlayer.play()


func hide_elements():
	%Rank.visible = false
	%Time.visible = false
	%Buttons.visible = false
	
	for child in %Rank.get_children():
		child.visible = false
	for child in %Time.get_children():
		child.visible = false
