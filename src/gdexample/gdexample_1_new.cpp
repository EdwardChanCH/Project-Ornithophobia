/**
 * @file gdexample_1_new.cpp
 * @author Chun Ho Chan
 * @brief This example class moves an image sprite around in a scene.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#include "gdexample_1_new.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void GDExample1New::_bind_methods() {
	ADD_SIGNAL(MethodInfo("position_changed_signal", PropertyInfo(Variant::VECTOR2, "new_pos")));
}

/**
 * @brief Construct a new GDExample1New::GDExample1New object.
 * 
 */
GDExample1New::GDExample1New() {
	amplitude = 10.0;
	speed = 1.0;
	time_passed = 0.0;
	time_emit = 0.0;
}

/**
 * @brief Destroy the GDExample1New::GDExample1New object.
 * 
 */
GDExample1New::~GDExample1New() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(GDExample1New)
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "amplitude"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(GDExample1New)
_GDEXPORT_GET(amplitude)
_GDEXPORT_GET(speed)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(GDExample1New)
_GDEXPORT_SET(amplitude)
_GDEXPORT_SET(speed)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void GDExample1New::_process(double delta) {
	// Keep track of how much time has passed
	time_passed += speed * delta;

	// Calculate a new position using a sine and cosine function
	Vector2 new_position = Vector2(
		amplitude + (amplitude * sin(time_passed * 2.0)), 
		amplitude + (amplitude * cos(time_passed * 1.5))
		);

	// Set sprite position
	set_position(new_position);

	// Send a signal "position_changed_signal" roughly every 0.2 second
	time_emit += delta;
	if (time_emit > 0.2) {
		emit_signal("position_changed_signal", new_position);
		time_emit = 0.0;
	}
}