/**
 * @file edward_main.h
 * @author  Chun Ho Chan (Edward)
 * @brief
 * @version
 * @date
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

	protected:
		static void _bind_methods(); // Must be declared

	public:
		EdwardMain();
		~EdwardMain();

		void _process(double delta) override;
		
		void _on_open_level_editor_button_pressed();
	};

} // namespace godot

#endif