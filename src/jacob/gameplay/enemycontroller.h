/**
 * @file enemycontroller.h
 * @author Jacob Couture
 * @brief Header file for the enemy controller class
 */

#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include <godot_cpp/classes/audio_stream_player.hpp>
#include "playercontroller.h"
#include "level_controller.h"
#include "debug.h"

namespace godot {

	class EnemyController : public Node2D {
		
		GDCLASS(EnemyController, Node2D) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		AnimatedSprite2D *deathAnim;
		int minKillSpeed = 600;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		EnemyController();
		~EnemyController();

		virtual void _ready() override;
        // virtual void _exit_tree() override;

		void _process(double delta) override;
		void _on_player_controller_entered(Node2D *body);
		
	};

} // namespace godot

#endif