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
    // CanvasLayer *ui = get_node<CanvasLayer>("../UI");
	// debugScene = ResourceLoader::get_singleton()->load("res://debug.tscn");
    // debugInstance = Node::cast_to<DebugController>(debugScene->instantiate());
    // // ui->add_child(debugInstance);
    // add_child(debugInstance);
    // debugInstance->add_debug_property("test", 1);
    // DebugController *debug = Node::cast_to<DebugController>(debugInstance->find_child("DebugController", true, false));
    // debug->add_debug_property("test", 1);
}

void LevelController::_exit_tree() {
    remove_child(debugInstance);
    debugInstance->queue_free();
}