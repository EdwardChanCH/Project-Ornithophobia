/**
 * @file debug_controller.h
 * @author Jacob Couture
 * @brief Header file for the DebugController class.
 */

#ifndef DEBUG_CONTROLLER_H
#define DEBUG_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include "globals.h"
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code. 
#include "debug.h"

namespace godot {

	class DebugController : public Node {
		
		GDCLASS(DebugController, Node) // A Godot macro for class inheritance. 

	protected:
		static void _bind_methods(); // Must be declared.

    private:
		// Reference to the container node that holds the property labels
        VBoxContainer *property_container;
		// Dictionary containing all tracked debug properties
		Dictionary properties;

	public:
		DebugController();
		~DebugController();

		virtual void _ready() override;
		virtual void _exit_tree() override;

		void _process(double delta) override;

	};

} // namespace godot

#endif