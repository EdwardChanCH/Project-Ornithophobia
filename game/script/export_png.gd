@tool
extends Node


func _notification(what: int) -> void:
	if (what == NOTIFICATION_EDITOR_POST_SAVE):
		
		var collision_nodes: Array = []
		#get_all_children(self, collision_nodes)
		get_all_children($PlayerList, collision_nodes)
		get_all_children($EnemyList, collision_nodes)
		
		for shape in collision_nodes:
			shape.visible = false
		
		var viewport = EditorInterface.get_editor_viewport_2d()
		var old_size = viewport.size
		viewport.global_canvas_transform = Transform2D(Vector2(1, 0), Vector2(0, 1), Vector2(0, 0))
		viewport.get_parent().stretch = false
		viewport.size = Vector2i(1920, 1080)
		#viewport.render_target_update_mode = viewport.UPDATE_ALWAYS
		
		await RenderingServer.frame_post_draw
		var thumbnail: Image = viewport.get_texture().get_image()
		thumbnail.save_png(scene_file_path.replace(".tscn", ".png"))
		viewport.size = old_size
		viewport.get_parent().stretch = true
		#viewport.render_target_update_mode = viewport.UPDATE_WHEN_VISIBLE
		
		for node in collision_nodes:
			node.visible = true


func get_all_children(node: Node, list: Array):
	for child in node.get_children():
		if (child.get_child_count() > 0):
			get_all_children(child, list)
		else:
			if (child != null and child is CollisionShape2D):
				list.append(child)
	return list
