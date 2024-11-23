/**
 * @file level.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-13
 * 
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "iprototype.h"

namespace godot {

	class Level : public IPrototype {
		
		GDCLASS(Level, IPrototype) // A Godot macro for class inheritance

	private:

	protected:
		static void _bind_methods(); // Must be declared

	public:
		Level();
		~Level();

		virtual IPrototype * clone() override;
	};

} // namespace godot

#endif