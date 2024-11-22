// This example class moves an image sprite around in a scene.

#include "mainmenu_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void MainMenuController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_play_button_pressed"), &MainMenuController::on_play_button_pressed);
	ClassDB::bind_method(D_METHOD("on_workshop_button_pressed"), &MainMenuController::on_workshop_button_pressed);
	ClassDB::bind_method(D_METHOD("on_settings_button_pressed"), &MainMenuController::on_settings_button_pressed);
	ClassDB::bind_method(D_METHOD("on_exit_button_pressed"), &MainMenuController::on_exit_button_pressed);
}

// Constructor. 
MainMenuController::MainMenuController() {
}

// Destructor. 
MainMenuController::~MainMenuController() {
}

void MainMenuController::on_play_button_pressed() {
	get_tree()->change_scene_to_file("res://scenes/level_select.tscn");
}

void MainMenuController::on_workshop_button_pressed() {
	UtilityFunctions::print("i wish there was a shop i could work in");
	// get_tree()->change_scene_to_file("res://scenes/workshop.tscn");
}

void MainMenuController::on_settings_button_pressed() {
	UtilityFunctions::print("no settings yet teehee");
	// No settings menu yet
	// get_tree()->change_scene_to_file("res://scenes/settings.tscn");
}

void MainMenuController::on_exit_button_pressed() {
	// should probably also auto-save
	get_tree()->quit();
}