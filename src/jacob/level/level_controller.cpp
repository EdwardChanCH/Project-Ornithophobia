/**
 * @file level_controller.cpp
 * @author Jacob Couture
 * @brief This class handles the initialization and control of a level.
 */

#include "level_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

/**
 * @brief Binding function for exposing methods/properties to Godot.
 */
void LevelController::_bind_methods() {
}

/**
 * @brief Constructor for LevelController
 */
LevelController::LevelController() {
}

/**
 * @brief Destructor for LevelController
 */
LevelController::~LevelController() {
}

/**
 * @brief Same as _ready() in GDScript.
 */
void LevelController::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(ProcessMode::PROCESS_MODE_ALWAYS);
        debugScene = ResourceLoader::get_singleton()->load("res://screen/debug.tscn");
        debugInstance = Node::cast_to<DebugController>(debugScene->instantiate());
        get_parent()->find_child("UI")->call_deferred("add_child", debugInstance);

        pauseScene = ResourceLoader::get_singleton()->load("res://screen/menu/pause_menu.tscn");
    }
}

/**
 * @brief Same as _input() in GDScript.
 */
void LevelController::_input(const Ref<InputEvent> &event) {
    if (event->is_action_pressed("escape")) {
        get_tree()->set_pause(!get_tree()->is_paused());
        if (get_tree()->is_paused()) {
            UtilityFunctions::print("ay we paused");
            pauseInstance = Node::cast_to<Control>(pauseScene->instantiate());
            // pauseInstance->set_z_index();
            get_parent()->find_child("UI")->call_deferred("add_child", pauseInstance);
        } else {
            UtilityFunctions::print("ay we unpaused");
            pauseInstance->queue_free();
        }
    }
}

/**
 * @brief Same as _exit_tree() in GDScript.
 */
void LevelController::_exit_tree() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        debugInstance->queue_free();
        pauseInstance->queue_free();
    }
}