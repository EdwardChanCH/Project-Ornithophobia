// DO NOT RENAME THIS FILE!
// Automatically generated with "recompile_register_types_cpp.py".
// This file registers all the classes contained in GDExtension's "custom" module with Godot.

#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

/*--------------------------------------------------*/
// Add exposed header files here.
#include "gdexample_1_new.h"
#include "gdexample_1_old.h"
#include "gdexample_2_new.h"
#include "gdexample_2_old.h"
#include "debug.h"
#include "debug_controller.h"
#include "level_controller.h"
#include "timecontroller.h"
#include "mainmenu_controller.h"
#include "playercontroller.h"
/*--------------------------------------------------*/

// Everything in GDExtension is defined within the namespace "godot".
using namespace godot;

static Debug *_debug;

// Load GDExtension's "custom" module, called by Godot.
void initialize_custom_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    /*--------------------------------------------------*/
    // Add exposed class names here.
    GDREGISTER_CLASS(GDExample1New);
    GDREGISTER_CLASS(GDExample1Old);
    GDREGISTER_CLASS(GDExample2New);
    GDREGISTER_CLASS(GDExample2Old);
    GDREGISTER_CLASS(Debug);
    GDREGISTER_CLASS(DebugController);
    GDREGISTER_CLASS(LevelController);
    GDREGISTER_CLASS(TimeController);
    GDREGISTER_CLASS(MainMenuController);
    GDREGISTER_CLASS(PlayerController);
    /*--------------------------------------------------*/

    Engine::get_singleton()->register_singleton("Debug", Debug::get_singleton());
}

// Unload GDExtension's "custom" module, called by Godot.
void uninitialize_custom_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    Engine::get_singleton()->unregister_singleton("Debug");
    memfree(_debug);
}

// This specifies that the function is defined elsewhere and uses the C-language calling convention.
extern "C" {
    
    // DO NOT RENAME THIS FUNCTION!
    // Entry function for initializing GDExtension.
    GDExtensionBool GDE_EXPORT cs3307_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        
        // Create an initialization object.
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
        
        // Register the initialize()/ uninitialize() functions of GDExtension.
        init_obj.register_initializer(initialize_custom_module);
        init_obj.register_terminator(uninitialize_custom_module);
        
        // Set the level of initialization (core, servers, scene, editor, or level).
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        
        // Return the initialization object.
        return init_obj.init();

    }

} // extern "C"