/**
 * @file level_editor_controller.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.2.0
 * @date 2024-11-17
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
		TypedArray<Callable> * undo_stack;
		TypedArray<Callable> * redo_stack;
		int action_index; // Index to insert a new action (in a new timeline)
		int undo_limit; // How many actions to keep track of

	protected:
		static void _bind_methods(); // Must be declared

	public:
		LevelEditorController();
		~LevelEditorController();

		Variant do_action(Callable new_action, Callable reverse_action);
		Variant undo_action();
		Variant redo_action();
		void clear_action();
		bool can_undo();
		bool can_redo();
		void _debug();
	};

} // namespace godot

#endif