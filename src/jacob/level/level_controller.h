#ifndef LEVEL_CONTROLLER_H
#define LEVEL_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include "globals.h"
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code. 
#include "debug_controller.h"

namespace godot {

	class LevelController : public Node2D {
		
		GDCLASS(LevelController, Node2D) // A Godot macro for class inheritance. 

	protected:
		static void _bind_methods(); // Must be declared.

    private:
        Ref<PackedScene> debugScene;
        DebugController* debugInstance;

	public:
		LevelController();
		~LevelController();

		virtual void _ready() override;
		virtual void _exit_tree() override;
		// virtual void _input(const Ref<InputEvent> &event) override;

	};

} // namespace godot

#endif