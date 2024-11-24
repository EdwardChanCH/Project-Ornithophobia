/**
 * @file level_editor_controller.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.3.0
 * @date 2024-11-22
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

		Ref<PackedScene> ui_scene;
		Node * ui_node;
		Level * level_node;

		const int default_undo_limit = 500;

		TypedArray<Callable> * undo_stack;
		TypedArray<Callable> * redo_stack;
		int action_index; // Index to insert a new action (in a new timeline)
		int undo_limit; // How many actions to keep track of
		String level_filepath; // Filepath of the current loaded level

	protected:
		static void _bind_methods(); // Must be declared

	public:
		LevelEditorController();
		~LevelEditorController();

		void _ready();

		Variant track_action(Callable new_action, Callable reverse_action);
		Variant undo_action();
		Variant redo_action();
		void clear_action();
		bool can_undo();
		bool can_redo();

		void load_level(String filepath);
		void save_level(String filepath);
		void quick_load_level();
		void quick_save_level();
		void unload_level();
		void reload_level();
		bool is_level_loaded();

		void toggle_physics(bool enable);
		Vector2i world_to_tile_pos(Vector2 world_pos, Vector2 tile_map_scale, Vector2i tile_size);
		Vector2 tile_to_world_pos(Vector2i tile_pos, Vector2 tile_map_scale, Vector2i tile_size);

		void _test_action(int n);
		void _debug();

		void _on_level_editor_ui_screen_left_clicked(Vector2 mouse_pos); // TODO
		void _on_level_editor_ui_screen_right_clicked(Vector2 mouse_pos); // TODO
	};

} // namespace godot

#endif