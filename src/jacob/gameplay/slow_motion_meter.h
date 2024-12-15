/**
 * @file slow_motion_meter.h
 * @author Jacob Couture
 * @brief Header file for the SlowMotionMeter class.
 */

#ifndef SLOW_MOTION_METER_H
#define SLOW_MOTION_METER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "debug.h"

namespace godot {

	class SlowMotionMeter : public ProgressBar {
		
		GDCLASS(SlowMotionMeter, ProgressBar) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		// Export variables
		float max_slow_time;
		float max_slow_time_value;
		float time_slow_factor;
		int regen_rate;
		int soft_max_value;

		// Class variables
		AnimationPlayer *animation_player;
		Timer *cooldown;
		bool can_fade_in;
		bool can_fade_out;
		bool can_regenerate = false;
		bool can_drain = true;
		bool can_slow_time;
		long start_press_time = 0;
		float max_time_slow_factor;
		float value;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		virtual void _ready() override;

		SlowMotionMeter();
		~SlowMotionMeter();

		void _process(double delta) override;
		void _on_cooldown_timer_timeout();
		void update_meter();
		void _on_results_showing();
	};

} // namespace godot

#endif