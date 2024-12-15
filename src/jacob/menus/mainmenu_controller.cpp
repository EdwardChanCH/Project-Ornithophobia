/**
 * @file mainmenu_controller.cpp
 * @author Jacob Couture
 * @brief This class handles menu controls.
 */

#include "mainmenu_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

/**
 * @brief Binding function for exposing methods/properties to Godot.
 */
void MainMenuController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_play_button_pressed"), &MainMenuController::on_play_button_pressed);
	ClassDB::bind_method(D_METHOD("on_workshop_button_pressed"), &MainMenuController::on_workshop_button_pressed);
	ClassDB::bind_method(D_METHOD("on_exit_button_pressed"), &MainMenuController::on_exit_button_pressed);
}

/**
 * @brief Constructor for MainMenuController
 */
MainMenuController::MainMenuController() {
}

/**
 * @brief Destructor for MainMenuController
 */
MainMenuController::~MainMenuController() {
}

/**
 * @brief Receiver for the play_button_pressed signal
 */
void MainMenuController::on_play_button_pressed() {
	SceneManager::get_instance()->load_new_scene(get_tree(), "res://screen/menu/level_select.tscn");
	// get_tree()->change_scene_to_file("res://screen/menu/level_select.tscn");
}

/**
 * @brief Receiver for the workshop_button_pressed signal
 */
void MainMenuController::on_workshop_button_pressed() {
	SceneManager::get_instance()->load_new_scene(get_tree(), "res://screen/menu/workshop_level_select.tscn");
}

/**
 * @brief Receiver for the exit_button_pressed signal
 */
void MainMenuController::on_exit_button_pressed() {
	// should probably also auto-save
	get_tree()->quit();
}