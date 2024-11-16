/**
 * @file edward_main.cpp
 * @author
 * @brief
 * @version
 * @date
 * 
 */

#include "edward_main.h"

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

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(EdwardMain)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(EdwardMain)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(EdwardMain)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void EdwardMain::_process(double delta) {
}

void EdwardMain::_on_open_level_editor_button_pressed() {
    UtilityFunctions::print("Changing scene to LevelEditor...");
    get_tree()->change_scene_to_file("res://level_editor_ui.tscn");
}