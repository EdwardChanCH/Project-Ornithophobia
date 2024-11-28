/**
 * @file edward_main.cpp
 * @author Chun Ho Chan (Edward)
 * @brief This main class is for loading Edward's level editor subsystem.
 * @version 0.2.0
 * @date 2024-11-22
 * 
 */

#include "edward_main.h"
#include "scene_manager.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void EdwardMain::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_edward_main_ui_open_level_editor_screen"), &EdwardMain::_on_edward_main_ui_open_level_editor_screen);
}

/**
 * @brief Construct a new Edward Main:: Edward Main object.
 * 
 */
EdwardMain::EdwardMain() {
    UtilityFunctions::print("Debug: EdwardMain constructor."); // TODO debug message
}

/**
 * @brief Destroy the Edward Main:: Edward Main object.
 * 
 */
EdwardMain::~EdwardMain() {
    UtilityFunctions::print("Debug: EdwardMain destructor."); // TODO debug message
    if (ui_node) {
        ui_scene.unref();
        ui_node = nullptr;
    }
}

/**
 * @brief Called when this node is ready in Godot's scene tree.
 * 
 */
void EdwardMain::_ready() {
    UtilityFunctions::print("Debug: EdwardMain ready."); // TODO debug message

    // Load Godot UI
    if (ResourceLoader::get_singleton()->exists(ui_filepath, "PackedScene")) {
        ui_scene = ResourceLoader::get_singleton()->load(ui_filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        ui_node = ui_scene.ptr()->instantiate();
        add_child(ui_node);

        // Connect Godot UI signals
        ui_node->connect("open_level_editor_screen", Callable(this, "_on_edward_main_ui_open_level_editor_screen"));
    }
}

/**
 * @brief Called when this node enters Godot's scene tree.
 * 
 */
void EdwardMain::_enter_tree() {
    UtilityFunctions::print("Debug: EdwardMain enter tree."); // TODO debug message
}

/**
 * @brief Called when this node exits Godot's scene tree.
 * 
 */
void EdwardMain::_exit_tree() {
    UtilityFunctions::print("Debug: EdwardMain exit tree."); // TODO debug message
}

// - - - Godot UI Signal Receivers - - -

void EdwardMain::_on_edward_main_ui_open_level_editor_screen() {
    SceneManager::get_instance()->load_new_scene(this->get_tree(), level_editor_filepath);
}