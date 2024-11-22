/**
 * @file edward_main.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-16
 * 
 */

#ifndef EDWARD_MAIN_H
#define EDWARD_MAIN_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node2d.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class EdwardMain : public Node2D {
		
		GDCLASS(EdwardMain, Node2D) // A Godot macro for class inheritance

	private:
		const String ui_filepath = "res://screen/edward_main_ui.tscn";
		const String level_editor_filepath = "res://screen/level_editor_ui.tscn";

	protected:
		static void _bind_methods(); // Must be declared

	public:
		EdwardMain();
		~EdwardMain();

		void _ready();
		void _enter_tree();
		void _exit_tree();
		
		void _on_edward_main_ui_open_level_editor();
	};

} // namespace godot

#endif