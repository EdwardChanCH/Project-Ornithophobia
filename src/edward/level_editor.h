/**
 * @file level_editor.h
 * @author Chun Ho Chan
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class LevelEditor : public Node {
		
		GDCLASS(LevelEditor, Node) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:

	protected:
		static void _bind_methods(); // Must be declared

	public:
		LevelEditor();
		~LevelEditor();

		void _process(double delta) override;

		void _on_file_button_pressed();
		void _on_edit_button_pressed();
		void _on_tile_button_pressed();
		void _on_entity_button_pressed();
	};

} // namespace godot

#endif