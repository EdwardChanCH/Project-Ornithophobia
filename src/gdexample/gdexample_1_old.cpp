// This example class moves an image sprite around in a scene.

#include "gdexample_1_old.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void GDExample1Old::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &GDExample1Old::set_amplitude);
	ClassDB::bind_method(D_METHOD("get_amplitude"), &GDExample1Old::get_amplitude);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");

	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &GDExample1Old::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &GDExample1Old::get_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");

	ADD_SIGNAL(MethodInfo("position_changed_signal", PropertyInfo(Variant::VECTOR2, "new_pos")));
}

// Constructor. 
GDExample1Old::GDExample1Old() {
	amplitude = 10.0;
	speed = 1.0;
	time_passed = 0.0;
	time_emit = 0.0;
}

// Destructor. 
GDExample1Old::~GDExample1Old() {
}

// Getter(s) for exported instance variables in Godot Editor. 
double GDExample1Old::get_amplitude() const {return amplitude;}
double GDExample1Old::get_speed() const {return speed;}

// Setter(s) for exported instance variables in Godot Editor. 
void GDExample1Old::set_amplitude(const double p_amplitude) {amplitude = p_amplitude;}
void GDExample1Old::set_speed(const double p_speed) {speed = p_speed;}

// Same as _process() in GDScript. 
void GDExample1Old::_process(double delta) {
	// Keep track of how much time has passed. 
	time_passed += speed * delta;

	// Calculate a new position using a sine and cosine function. 
	Vector2 new_position = Vector2(
		amplitude + (amplitude * sin(time_passed * 2.0)), 
		amplitude + (amplitude * cos(time_passed * 1.5))
		);

	// Set sprite position. 
	set_position(new_position);

	// Send a signal "position_changed_signal" roughly every 0.2 second. 
	time_emit += delta;
	if (time_emit > 0.2) {
		emit_signal("position_changed_signal", new_position);
		time_emit = 0.0;
	}
}