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
        VBoxContainer *property_container;
		Label *debugText;
		Dictionary *properties;

	public:
		DebugController();
		~DebugController();

		virtual void _ready() override;
		virtual void _exit_tree() override;

		void _process(double delta) override;

	};

} // namespace godot

#endif