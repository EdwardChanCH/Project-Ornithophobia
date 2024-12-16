# Author: Jacob Couture
# Description: Controller for the Results Screen.

extends Control


# Called when the node is ready in the scene tree.
func _ready() -> void:
	visible = false
	hide_elements()


# Receiver function for the show_results signal. Shows all the relevant information in the correct order
func _on_results_screen_show(level_metadata: Dictionary):
	get_tree().paused = true
	hide_elements()
	print(level_metadata)
	%LevelComplete.text = level_metadata.get("level_name") + " COMPLETE!"
	%LevelTime.text = level_metadata.get("level_time")
	%RankIcon.texture = load(level_metadata.get("rank_icon"))
	%RankFlavourText.text = level_metadata.get("rank_flavour_text")
	
	visible = true
	$AnimationPlayer.play("fade_in")
	await get_tree().create_timer(0.8).timeout
	
	# If this is a custom level, move the Time nodes to the center of the screen
	if (level_metadata.get("level_author") != "null"):
		%Time.position = Vector2(808, 456)
	
	show_result_attribute(%TimeLabel)
	await get_tree().create_timer(0.6).timeout
	show_result_attribute(%LevelTime)
	if (Global.read_formatted_time(%LevelTime.text) <= Global.read_formatted_time(level_metadata.get("best_time"))):
		%BestTimeCrown.visible = true
	
	# If this is a story level, display the rank information
	if (level_metadata.get("level_author") == "null"):
		await get_tree().create_timer(0.8).timeout
		show_result_attribute(%RankLabel)
		await get_tree().create_timer(0.6).timeout
		show_result_attribute(%RankIcon)
		await get_tree().create_timer(0.8).timeout
		show_result_attribute(%RankFlavourText)
	
	await get_tree().create_timer(0.5).timeout
	%Buttons.visible = true


func show_result_attribute(node: Control):
	node.visible = true
	$AudioStreamPlayer.play()


func hide_elements():
	%Buttons.visible = false
	
	for child in %Rank.get_children():
		child.visible = false
	for child in %Time.get_children():
		child.visible = false
