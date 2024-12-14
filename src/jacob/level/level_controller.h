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
#include "enemycontroller.h"
#include "debug.h"
#include "level.h"

namespace godot {

	class LevelController : public Node2D {
		
		GDCLASS(LevelController, Node2D) // A Godot macro for class inheritance. 
		_GDEXPORT

	protected:
		static void _bind_methods(); // Must be declared.

    private:
		Dictionary level_metadata;
		String level_name;
		String level_author;
		String level_icon;
		String best_time = "--:--.--";
		PackedStringArray rank_times;
		String rank_icon;
		String rank_flavour_text;
		Array gold_rank_text;
		Array silver_rank_text;
		Array bronze_rank_text;
		String no_rank_text = "BETTER LUCK NEXT TIME!";
		String level_path;

		Level* levelNode;
		CanvasLayer* gameplayUI;
        // Ref<PackedScene> levelUIScene;
        Control* levelUINode;
		// Ref<PackedScene> pauseScene;
		Control* pauseScreenNode;
		// Ref<PackedScene> resultsScreenScene;
		Control* resultsScreenNode;
		Camera2D* camera;
		
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
		void _on_return_button_pressed();
		void _on_retry_button_pressed();

		void calculate_best_time();
		int read_formatted_time(String time);
		void calculate_rank();
		void set_level(String level_path);
	};

} // namespace godot

#endif