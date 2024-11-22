/**
 * @file enemycontroller.h
 * @author
 * @brief
 * @version
 * @date
 * 
 */

#ifndef _ENEMYCONTROLLER__H
#define _ENEMYCONTROLLER__H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class EnemyController : public Node2D {
		
		GDCLASS(EnemyController, Node2D) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		AnimatedSprite2D *deathAnim;

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