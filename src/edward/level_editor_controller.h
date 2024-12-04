/**
 * @file level_editor_controller.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.3.1
 * @date 2024-11-25
 * 
 */

#ifndef LEVEL_EDITOR_CONTROLLER_H
#define LEVEL_EDITOR_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "level.h"

namespace godot {

	class LevelEditorController : public Node {
		
		GDCLASS(LevelEditorController, Node) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		const String ui_filepath = "res://screen/level_editor_ui.tscn";
		const String playtest_filepath = "res://screen/playtest.tscn";
		const int default_undo_limit = 500;

		Ref<PackedScene> ui_scene;
		Node * ui_node;
		Level * level_node;
		String level_filepath; // Filepath of the current loaded level

		TypedArray<Callable> * undo_stack;
		TypedArray<Callable> * redo_stack;
		int action_index; // Index to insert a new action (in a new timeline)
		int quick_save_action_index; // Action index of the quick save
		int undo_limit; // How many actions to keep track of
		bool has_quick_saved; // Has quick saved at least once; old quick save files are not deleted when exiting game

		bool physics_on; // Physics toggle
		bool tracking_on; // Tracking undo/ redo toggle

		Vector2i tile_id; // Tile ID in the tile set (column, row)
		bool tile_flip_h; // Tile flipped horizontally
		bool tile_flip_v; // Tile flipped vertically
		bool tile_flip_d; // Tile flipped diagonally (transpose)

	protected:
		static void _bind_methods(); // Must be declared

	public:
		LevelEditorController();
		~LevelEditorController();

		void _ready();

		void track_action(Callable redo_action, Callable undo_action);
		void clear_action();
		Variant undo_action();
		Variant redo_action();
		bool can_undo();
		bool can_redo();

		void set_level_filepath(String filepath);
		void load_new_level(String filepath);
		void load_level(String filepath);
		void save_level(String filepath);
		void quick_load_level();
		void quick_save_level();
		void unload_level();
		void reload_level();
		bool is_level_loaded();

		void toggle_physics();
		Vector2i world_to_tile_pos(Vector2 world_pos, Vector2 tile_map_scale, Vector2i tile_size);
		Vector2 tile_to_world_pos(Vector2i tile_pos, Vector2 tile_map_scale, Vector2i tile_size);
		int to_tile_alt(bool flip_h, bool flip_v, bool flip_d);
		
		void flip_tile(int mode);
		void rotate_tile(bool clockwise);
		void cycle_tile(bool next);

		void add_tile(Vector2 mouse_pos);
		void remove_tile(Vector2 mouse_pos);
		void replace_tile(Vector2i tile_pos, int source_id, Vector2i tile_id, int tile_flags); // TODO
		void add_scene_object(Vector2 mouse_pos, String list_name, String scene_path);
		void add_player(Vector2 mouse_pos);
		void add_enemy(Vector2 mouse_pos);
		void add_entity(Vector2 mouse_pos);

		void _test_action(String s, int n);
		void _debug();

		const String default_level_filepath = "res://level/level_default_empty.tscn";
	};

} // namespace godot

#endif