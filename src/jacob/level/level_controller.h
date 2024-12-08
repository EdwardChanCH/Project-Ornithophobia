/**
 * @file level_controller.h
 * @author Jacob Couture
 * @brief Header file for the LevelController class
 */

#ifndef LEVEL_CONTROLLER_H
#define LEVEL_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include "globals.h"
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code. 
#include "debug_controller.h"
#include "scene_manager.h"
#include "level_timer.h"
#include "playercontroller.h"

namespace godot {

	class LevelController : public Node2D {
		
		GDCLASS(LevelController, Node2D) // A Godot macro for class inheritance. 
		_GDEXPORT

	protected:
		static void _bind_methods(); // Must be declared.

    private:
		String level_name;
		String level_author;
		String level_icon_path;
		String best_time = "--:--.--";
		PackedStringArray rank_times;
		String rank_icon_path;

        Ref<PackedScene> levelUIScene;
        Control* levelUIInstance;
		Ref<PackedScene> pauseScene;
		Control* pauseInstance;
		CanvasLayer* gameplayUI;
		Ref<PackedScene> resultsScreenScene;
		Control* resultsScreenInstance;
		
		Node* enemyList;
		int numEnemies = -1;
		int totalEnemies = -1;

		Node* playerList;
		
		float timeScaleFactor = 0.0016;
		long slowLength;

	public:
		LevelController();
		~LevelController();

		virtual void _ready() override;
		virtual void _exit_tree() override;
		virtual void _input(const Ref<InputEvent> &event) override;

		void _process(double delta) override;
		void _update_enemy_count();
		void _results_slow_time();

		void calculate_best_time();
		int read_formatted_time(String time);

		void calculate_rank();
	};

} // namespace godot

#endif