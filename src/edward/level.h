/**
 * @file level.h
 * @author
 * @brief
 * @version
 * @date
 * 
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class Level : public Node {
		
		GDCLASS(Level, Node) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:

	protected:
		static void _bind_methods(); // Must be declared

	public:
		Level();
		~Level();

		void _process(double delta) override;
	};

} // namespace godot

#endif