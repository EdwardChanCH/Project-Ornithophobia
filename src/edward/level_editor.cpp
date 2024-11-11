/**
 * @file level_editor.cpp
 * @author Chun Ho Chan
 * @brief This class represents the level editor.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#include "level_editor.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void LevelEditor::_bind_methods() {
}

/**
 * @brief Construct a new Level Editor:: Level Editor object.
 * 
 */
LevelEditor::LevelEditor() {
}

/**
 * @brief Destroy the Level Editor:: Level Editor object.
 * 
 */
LevelEditor::~LevelEditor() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(LevelEditor)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelEditor)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelEditor)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void LevelEditor::_process(double delta) {
}