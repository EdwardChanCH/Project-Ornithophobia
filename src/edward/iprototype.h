/**
 * @file iprototype.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-13
 * 
 */

#ifndef IPROTOTYPE_H
#define IPROTOTYPE_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class IPrototype : public Node {
		
		GDCLASS(IPrototype, Node) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:

	protected:
		static void _bind_methods(); // Must be declared

	public:
		virtual IPrototype * clone() = 0;
	};

} // namespace godot

#endif