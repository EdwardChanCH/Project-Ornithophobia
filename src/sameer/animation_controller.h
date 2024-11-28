/**
 * @file animation_controller.h
 * @author Sameer Bugti
 * @brief
 * @version 1
 * @date November 25th, 2024.
 * 
 */

#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "playercontroller.h"

namespace godot {

	class AnimationController : public AnimatedSprite2D {
		
		GDCLASS(AnimationController, AnimatedSprite2D) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
        PlayerController *parent;	//grabs parent node for player characteristics
		AnimatedSprite2D *arms;	//grabs child node for arm sprite

	protected:
		static void _bind_methods(); // Must be declared

	public:
        virtual void _ready() override;
        
		AnimationController();
		~AnimationController();

		void _process(double delta) override;
	};

} // namespace godot

#endif