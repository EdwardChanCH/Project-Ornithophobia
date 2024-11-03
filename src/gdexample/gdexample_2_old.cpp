// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "gdexample_2_old.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void GDExample2Old::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_gd_example_1_old_position_changed_signal", "p_position"), &GDExample2Old::_on_gd_example_1_old_position_changed_signal);
}

// Constructor. 
GDExample2Old::GDExample2Old() {
    counter = 0;
}

// Destructor. 
GDExample2Old::~GDExample2Old() {
}

void GDExample2Old::print_received_signal(Vector2 p_position) {
    // Note: p_position is unused here. 
    counter += 1;
    set_visible_characters(counter);
    if (counter >= 26) {
        counter = 0;
    }
}

// Signal Listener(s): _on_<node_name>_<signal_name>(optional parameters)

void GDExample2Old::_on_gd_example_1_old_position_changed_signal(Vector2 p_position) {
    print_received_signal(p_position);
}