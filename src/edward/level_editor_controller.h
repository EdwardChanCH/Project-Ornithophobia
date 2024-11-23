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

namespace godot {

	class LevelEditorController : public Node {
		
		GDCLASS(LevelEditorController, Node) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		const String ui_filepath = "res://screen/level_editor_ui.tscn";
		const String playtest_filepath = "res://screen/playtest.tscn";

		Ref<PackedScene> ui_scene;
		Node * ui_node;
		Ref<PackedScene> level_scene;
		Node * level_node;

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

		Variant do_action(Callable new_action, Callable reverse_action);
		Variant undo_action();
		Variant redo_action();
		void clear_action();
		bool can_undo();
		bool can_redo();

		void load_level(String filepath);
		void save_level(String filepath);
		void unload_level();
		void reload_level();

		void _test_print(String s, int n);
		void _debug();

		void _on_level_editor_ui_open_previous_screen();
		void _on_level_editor_ui_playtest_button_toggled(bool active);
		void _on_level_editor_ui_debug_level_editor_controller();
		void _on_level_editor_ui_test_action_button_pressed(int n);
		void _on_level_editor_ui_undo_button_pressed();
		void _on_level_editor_ui_redo_button_pressed();
		void _on_level_editor_ui_load_level_path_selected(String filepath);
		void _on_level_editor_ui_save_level_path_selected(String filepath);
		void _on_level_editor_ui_reload_level_button_pressed();
		void _on_level_editor_ui_unload_level_button_pressed();
		void _on_level_editor_ui_screen_left_clicked(Vector2 mouse_pos);
		void _on_level_editor_ui_screen_right_clicked(Vector2 mouse_pos);
	};

} // namespace godot

#endif