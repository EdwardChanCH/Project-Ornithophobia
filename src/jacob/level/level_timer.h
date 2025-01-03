/**
 * @file level_timer.h
 * @author Jacob Couture
 * @brief Header file for the LevelTimer class.
 */

#ifndef LEVEL_TIMER_H
#define LEVEL_TIMER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class LevelTimer : public Label {
		
		GDCLASS(LevelTimer, Label) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		// The time this timer began
		uint64_t start_time;
		// The amount of time this timer has spent paused
		uint64_t pause_time;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		LevelTimer();
		~LevelTimer();

		void _ready() override;
		void _process(double delta) override;

		String format_time(uint64_t time);
	};

} // namespace godot

#endif