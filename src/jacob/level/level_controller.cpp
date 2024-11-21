// This example class moves an image sprite around in a scene.

#include "level_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void LevelController::_bind_methods() {
}

// Constructor. 
LevelController::LevelController() {
}

// Destructor. 
LevelController::~LevelController() {
}

void LevelController::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        debugScene = ResourceLoader::get_singleton()->load("res://scenes/debug.tscn");
        debugInstance = Node::cast_to<DebugController>(debugScene->instantiate());
        get_parent()->find_child("UI")->call_deferred("add_child", debugInstance);
    }
}

void LevelController::_input(const Ref<InputEvent> &event) {
    if (event->is_action_pressed("escape")) {
        get_tree()->change_scene_to_file("res://scenes/main_menu.tscn");
    }
}

void LevelController::_exit_tree() {
    if (!Engine::get_singleton()->is_editor_hint())
        debugInstance->queue_free();
}