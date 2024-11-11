/**
 * @file gdexample_2_old.cpp
 * @author Chun Ho Chan
 * @brief This example class receives a signal from a GDExampleNode and output some text to the terminal.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#include "gdexample_2_old.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void GDExample2Old::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_gd_example_1_old_position_changed_signal", "p_position"), &GDExample2Old::_on_gd_example_1_old_position_changed_signal);
}

/**
 * @brief Construct a new GDExample2Old::GDExample2Old object.
 * 
 */
GDExample2Old::GDExample2Old() {
    counter = 0;
}

/**
 * @brief Destroy the GDExample2Old::GDExample2Old object.
 * 
 */
GDExample2Old::~GDExample2Old() {
}

/**
 * @brief Function that prints moving text on the screen.
 * 
 * @param p_position p_position is unused here
 */
void GDExample2Old::print_received_signal(Vector2 p_position) {
    counter += 1;
    set_visible_characters(counter);
    if (counter >= 26) {
        counter = 0;
    }
}

/**
 * @brief Receives the "position_changed_signal" signal.
 * Signal Listener: _on_<node_name>_<signal_name>(optional parameters)
 * 
 * @param p_position Signal argument
 */
void GDExample2Old::_on_gd_example_1_old_position_changed_signal(Vector2 p_position) {
    print_received_signal(p_position);
}