/**
 * @file register_types.cpp
 * @author GDExtension
 * @brief This file registers all the classes contained in GDExtension's "custom" module with Godot.
 * Automatically generated by "generate_register_types_cpp.py".
 * DO NOT RENAME THIS FILE!
 * 
 */

#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

/*--------------------------------------------------*/
// Add exposed header files here
// #include ">_< headers_here >_<"
#include "gdexample_1_new.h"
#include "gdexample_1_old.h"
#include "gdexample_2_new.h"
#include "gdexample_2_old.h"
#include "level_timer.h"
#include "animation_controller.h"
#include "iprototype.h"
#include "level.h"
#include "level_editor_controller.h"
#include "edward_main.h"
#include "scene_manager.h"
#include "slow_motion_meter.h"
#include "enemycontroller.h"
#include "debug.h"
#include "debug_controller.h"
#include "level_controller.h"
#include "mainmenu_controller.h"
#include "playercontroller.h"
/*--------------------------------------------------*/

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Load GDExtension's "custom" module, called by Godot.
 * 
 * @param p_level Module initialization level
 */
void initialize_custom_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    /*--------------------------------------------------*/
    // Add exposed class names here
    // GDREGISTER_CLASS(>_< classes_here >_<);
    GDREGISTER_CLASS(GDExample1New);
    GDREGISTER_CLASS(GDExample1Old);
    GDREGISTER_CLASS(GDExample2New);
    GDREGISTER_CLASS(GDExample2Old);
    GDREGISTER_CLASS(LevelTimer);
    GDREGISTER_CLASS(AnimationController);
    GDREGISTER_ABSTRACT_CLASS(IPrototype);
    GDREGISTER_CLASS(Level);
    GDREGISTER_CLASS(LevelEditorController);
    GDREGISTER_CLASS(EdwardMain);
    GDREGISTER_CLASS(SceneManager);
    GDREGISTER_CLASS(SlowMotionMeter);
    GDREGISTER_CLASS(EnemyController);
    GDREGISTER_CLASS(Debug);
    GDREGISTER_CLASS(DebugController);
    GDREGISTER_CLASS(LevelController);
    GDREGISTER_CLASS(MainMenuController);
    GDREGISTER_CLASS(PlayerController);
    /*--------------------------------------------------*/
}

/**
 * @brief Unload GDExtension's "custom" module, called by Godot.
 * 
 * @param p_level Module initialization level
 */
void uninitialize_custom_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

// This specifies that the function is defined elsewhere and uses the C-language calling convention
extern "C" {

    /**
     * @brief Entry function for initializing GDExtension.
     * DO NOT RENAME THIS FUNCTION!
     * 
     * @param p_get_proc_address
     * @param p_library
     * @param r_initialization
     * @return GDExtensionBool
     */
    GDExtensionBool GDE_EXPORT cs3307_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        // Create an initialization object
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
        
        // Register the initialize()/ uninitialize() functions of GDExtension
        init_obj.register_initializer(initialize_custom_module);
        init_obj.register_terminator(uninitialize_custom_module);
        
        // Set the level of initialization (core, servers, scene, editor, or level)
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        
        // Return the initialization object
        return init_obj.init();
    }

} // extern "C"