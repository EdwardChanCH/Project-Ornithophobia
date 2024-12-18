class_name Tile2D
extends Node2D
## This class represents a tile as a Sprite2D.
##
## This script edits the tile type and orientation.
##
## Author: Chun Ho Chan (Edward)
## Date:   2024-12-13


@export var tile_sprite: Sprite2D


# Change tile type
func set_tile_type(atlas_coords: Vector2i) -> void:
	var texture: AtlasTexture = tile_sprite.texture
	var texture_size: Vector2 = texture.region.size
	texture.region = Rect2(atlas_coords * texture_size.x, texture_size)
	pass


# Change tile rotation ID
func set_tile_alt(tile_alt: int) -> void:
	var rotation_id: int = 0
	var tile_flip_h: bool = ((tile_alt & TileSetAtlasSource.TRANSFORM_FLIP_H) > 0)
	var tile_flip_v: bool = ((tile_alt & TileSetAtlasSource.TRANSFORM_FLIP_V) > 0)
	var tile_flip_d: bool = ((tile_alt & TileSetAtlasSource.TRANSFORM_TRANSPOSE) > 0)
	
	# Encode
	if (!tile_flip_h and !tile_flip_v and !tile_flip_d):
		rotation_id = 0
	elif (tile_flip_h and !tile_flip_v and tile_flip_d):
		rotation_id = 1
	elif (tile_flip_h and tile_flip_v and !tile_flip_d):
		rotation_id = 2
	elif (!tile_flip_h and tile_flip_v and tile_flip_d):
		rotation_id = 3
	elif (tile_flip_h and !tile_flip_v and !tile_flip_d):
		rotation_id = 4
	elif (tile_flip_h and tile_flip_v and tile_flip_d):
		rotation_id = 5
	elif (!tile_flip_h and tile_flip_v and !tile_flip_d):
		rotation_id = 6
	elif (!tile_flip_h and !tile_flip_v and tile_flip_d):
		rotation_id = 7
	
	# Decode
	if (rotation_id >= 0 and rotation_id <= 3):
		tile_sprite.flip_h = false
		self.rotation_degrees = rotation_id * 90
	elif (rotation_id >= 4 and rotation_id <= 7):
		tile_sprite.flip_h = true
		self.rotation_degrees = (rotation_id - 4) * 90
	
	pass
