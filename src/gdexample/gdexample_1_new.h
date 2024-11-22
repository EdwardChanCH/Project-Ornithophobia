/**
 * @file gdexample_1_new.h
 * @author Chun Ho Chan
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#ifndef GDEXAMPLE_1_NEW_H
#define GDEXAMPLE_1_NEW_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/sprite2d.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code

namespace godot {

	class GDExample1New : public Sprite2D {
		
		GDCLASS(GDExample1New, Sprite2D) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		double amplitude;
		double speed;
		double time_passed;
		double time_emit;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		GDExample1New();
		~GDExample1New();

		void _process(double delta) override;
	};

} // namespace godot

#endif