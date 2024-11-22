/**
 * @file edward_main.cpp
 * @author Chun Ho Chan (Edward)
 * @brief This main class is for testing Edward's systems.
 * @version 0.1.0
 * @date 2024-11-16
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
    ClassDB::bind_method(D_METHOD("_on_edward_main_ui_open_level_editor"), &EdwardMain::_on_edward_main_ui_open_level_editor);
}

/**
 * @brief Construct a new Edward Main:: Edward Main object.
 * 
 */
EdwardMain::EdwardMain() {
    UtilityFunctions::print("EdwardMain constructor."); // TODO debug message
    
}

/**
 * @brief Destroy the Edward Main:: Edward Main object.
 * 
 */
EdwardMain::~EdwardMain() {
    UtilityFunctions::print("EdwardMain destructor."); // TODO debug message
    
}

/**
 * @brief Called when this node is ready in Godot's scene tree.
 * 
 */
void EdwardMain::_ready() {
    UtilityFunctions::print("EdwardMain ready."); // TODO debug message

    if (ResourceLoader::get_singleton()->exists(ui_filepath, "PackedScene")) {
        // Load UI node to scene tree
        Ref<PackedScene> ui_scene = ResourceLoader::get_singleton()->load(ui_filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        Node * ui_node = ui_scene.ptr()->instantiate();
        add_child(ui_node);

        // Connect UI node signals
        ui_node->connect("open_level_editor", Callable(this, "_on_edward_main_ui_open_level_editor"));
    }
}

/**
 * @brief Called when this node enters Godot's scene tree.
 * 
 */
void EdwardMain::_enter_tree() {
    UtilityFunctions::print("EdwardMain enter tree."); // TODO debug message
}

/**
 * @brief Called when this node exits Godot's scene tree.
 * 
 */
void EdwardMain::_exit_tree() {
    UtilityFunctions::print("EdwardMain exit tree."); // TODO debug message
}

/**
 * @brief Signal receiver.
 * 
 */
void EdwardMain::_on_edward_main_ui_open_level_editor() {
    SceneManager::get_instance()->load_new_scene(this->get_tree(), level_editor_filepath);
}