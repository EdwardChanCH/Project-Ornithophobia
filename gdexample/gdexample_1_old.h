/**
 * @file gdexample_1_old.h
 * @author Chun Ho Chan
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#ifndef GDEXAMPLE_1_OLD_H
#define GDEXAMPLE_1_OLD_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/sprite2d.hpp> // Bindings to the parent class

namespace godot {

	class GDExample1Old : public Sprite2D {

		GDCLASS(GDExample1Old, Sprite2D) // A Godot macro for class inheritance

	private:
		double amplitude;
		double speed;
		double time_passed;
		double time_emit;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		GDExample1Old();
		~GDExample1Old();

		void _process(double delta) override;

		void set_amplitude(const double p_amplitude);
		double get_amplitude() const;
		void set_speed(const double p_speed);
		double get_speed() const;
	};

} // namespace godot

#endif