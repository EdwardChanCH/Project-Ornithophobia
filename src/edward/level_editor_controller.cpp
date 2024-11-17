/**
 * @file level_editor_controller.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Mediator Pattern (also MVC Pattern).
 * This class controlls all communication between Godot UI (view) and Level objects (model).
 * @version 0.2.0
 * @date 2024-11-17
 * 
 */

#include "level_editor_controller.h"
#include "scene_manager.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(LevelEditorController)
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "undo_limit"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelEditorController)
_GDEXPORT_GET(undo_limit)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelEditorController)
_GDEXPORT_SET(undo_limit)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void LevelEditorController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("do_action", "new_action", "reverse_action"), &LevelEditorController::do_action);
    ClassDB::bind_method(D_METHOD("undo_action"), &LevelEditorController::undo_action);
    ClassDB::bind_method(D_METHOD("redo_action"), &LevelEditorController::redo_action);
    ClassDB::bind_method(D_METHOD("clear_action"), &LevelEditorController::clear_action);
    ClassDB::bind_method(D_METHOD("can_undo"), &LevelEditorController::can_undo);
    ClassDB::bind_method(D_METHOD("can_redo"), &LevelEditorController::can_redo);
    ClassDB::bind_method(D_METHOD("_debug"), &LevelEditorController::_debug);
}

/**
 * @brief Construct a new Level Editor Controller:: Level Editor Controller object.
 * 
 */
LevelEditorController::LevelEditorController() {
    undo_stack = memnew(TypedArray<Callable>);
    redo_stack = memnew(TypedArray<Callable>);
    action_index = 0;
    undo_limit = 100;
}

/**
 * @brief Destroy the Level Editor Controller:: Level Editor Controller object.
 * 
 */
LevelEditorController::~LevelEditorController() {
    memdelete(undo_stack);
    undo_stack = nullptr;
    memdelete(redo_stack);
    redo_stack = nullptr;
}

Variant LevelEditorController::do_action(Callable new_action, Callable reverse_action) {
    // Remove actions in old timeline
    while (action_index < redo_stack->size()) {
        undo_stack->pop_back();
        redo_stack->pop_back();
    }

    // Track new action
    undo_stack->push_back(reverse_action);
    redo_stack->push_back(new_action);
    ++action_index;

    if (action_index > undo_limit) {
        // Remove oldest action
        undo_stack->pop_front();
        redo_stack->pop_front();
        --action_index;
    }

    // Execute action
    return new_action.call();
}

Variant LevelEditorController::undo_action() {
    if (!can_undo()) {
        // No more undo available
        UtilityFunctions::print("Warning: No undo available.");
        return Variant::NIL;
    }

    // Find undo action
    --action_index;
    Callable action = (*undo_stack)[action_index];

    // Execute action
    return action.call();
}

Variant LevelEditorController::redo_action() {
    if (!can_redo()) {
        // No more redo available
        UtilityFunctions::print("Warning: No redo available.");
        return Variant::NIL;
    }

    // Find redo action
    Callable action = (*redo_stack)[action_index];
    ++action_index;

    // Execute action
    return action.call();
}

void LevelEditorController::clear_action() {
    undo_stack->clear();
    redo_stack->clear();
    action_index = 0;
}

bool LevelEditorController::can_undo() {
    // Check if there is at least 1 undoable action
    return (action_index > 0);
}

bool LevelEditorController::can_redo() {
    // Check if there is at least 1 redoable action
    return (action_index < redo_stack->size());
}

void LevelEditorController::_debug() {
    UtilityFunctions::print("> Debug start.");

    UtilityFunctions::print("> Index:", action_index);
    UtilityFunctions::print("> Printing 'redo_stack':");
    for (int i = 0; i < redo_stack->size(); ++i) {
        UtilityFunctions::print((*redo_stack)[i]);
    }

    UtilityFunctions::print("> Printing 'undo_stack':");
    for (int i = 0; i < undo_stack->size(); ++i) {
        UtilityFunctions::print((*undo_stack)[i]);
    }

    UtilityFunctions::print("> Debug end.");
}