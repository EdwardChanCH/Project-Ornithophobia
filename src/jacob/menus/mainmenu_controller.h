/**
 * @file mainmenu_controller.h
 * @author Jacob Couture
 * @brief Header file for the MainMenuController class.
 */

#ifndef MAINMENU_CONTROLLER_H
#define MAINMENU_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include "globals.h"
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code.
#include "scene_manager.h"

namespace godot {

	class MainMenuController : public Node {
		
		GDCLASS(MainMenuController, Node) // A Godot macro for class inheritance. 

	protected:
		static void _bind_methods(); // Must be declared. 

	public:
		MainMenuController();
		~MainMenuController();

		void on_play_button_pressed();
		void on_workshop_button_pressed();
		void on_settings_button_pressed();
		void on_exit_button_pressed();

	};

} // namespace godot

#endif