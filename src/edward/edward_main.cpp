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
    ClassDB::bind_method(D_METHOD("_on_open_level_editor_button_pressed"), &EdwardMain::_on_open_level_editor_button_pressed);
}

/**
 * @brief Construct a new Edward Main:: Edward Main object.
 * 
 */
EdwardMain::EdwardMain() {
}

/**
 * @brief Destroy the Edward Main:: Edward Main object.
 * 
 */
EdwardMain::~EdwardMain() {
}

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void EdwardMain::_process(double delta) {
}

void EdwardMain::_on_open_level_editor_button_pressed() {
    UtilityFunctions::print("Changing scene to LevelEditor...");
    SceneManager::get_instance()->load_new_scene(this->get_tree(), "res://level_editor_ui.tscn");
    // get_tree()->change_scene_to_file("res://level_editor_ui.tscn");
}