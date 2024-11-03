// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "gdexample_2_new.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void GDExample2New::_bind_methods() {
    ClassDB::bind_method(D_METHOD("print_received_signal", "p_position"), &GDExample2New::print_received_signal);
}

// Constructor. 
GDExample2New::GDExample2New() {
    counter = 0;
}

// Destructor. 
GDExample2New::~GDExample2New() {
}

// Directly receives the "position_changed_signal" signal. Not recommended due to poor readability, but possible. 
// Function that prints moving text on the screen. 
void GDExample2New::print_received_signal(Vector2 p_position) {
    // Note: p_position is unused here. 
    counter += 1;
    set_visible_characters(counter);
    if (counter >= 10) {
        counter = 0;
    }
}