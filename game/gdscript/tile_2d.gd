class_name Tile2D
extends Node2D


@export var tile_sprite: Sprite2D


# Emulates a cell in TileMapLayer
func set_cell(atlas_coords: Vector2i, rotation_id: int) -> void:
	# Change sprite
	var texture: AtlasTexture = tile_sprite.texture
	texture.region = Rect2(atlas_coords * 16, Vector2(16, 16))
	
	# Change rotation
	if (rotation_id >= 0 and rotation_id <= 3):
		tile_sprite.flip_h = false
		self.rotation_degrees = rotation_id * 90
	elif (rotation_id >= 4 and rotation_id <= 7):
		tile_sprite.flip_h = true
		self.rotation_degrees = (rotation_id - 4) * 90
	else:
		tile_sprite.flip_h = false
		self.rotation_degrees = 0
	
	pass
