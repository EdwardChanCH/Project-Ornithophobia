/**
 * @file gdexample_2_new.cpp
 * @author Chun Ho Chan
 * @brief This example class receives a signal from a GDExampleNode and output some text to the terminal.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#include "gdexample_2_new.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void GDExample2New::_bind_methods() {
    ClassDB::bind_method(D_METHOD("print_received_signal", "p_position"), &GDExample2New::print_received_signal);
}

/**
 * @brief Construct a new GDExample2New::GDExample2New object.
 * 
 */
GDExample2New::GDExample2New() {
    counter = 0;
}

/**
 * @brief Destroy the GDExample2New::GDExample2New object.
 * 
 */
GDExample2New::~GDExample2New() {
}

/**
 * @brief Function that prints moving text on the screen.
 * Directly receives the "position_changed_signal" signal.
 * Not recommended due to poor readability, but possible.
 * 
 * @param p_position p_position is unused here
 */
void GDExample2New::print_received_signal(Vector2 p_position) {
    counter += 1;
    set_visible_characters(counter);
    if (counter >= 10) {
        counter = 0;
    }
}