/**
 * @file debug_controller.cpp
 * @author Jacob Couture
 * @brief This class handles the creation and rendering of properties on the in-game debug menu
 */

#include "debug_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

/**
 * @brief Binding function for exposing methods/properties to Godot.
 */
void DebugController::_bind_methods() {
}

/**
 * @brief Constructor for DebugController
 */
DebugController::DebugController() {
}

/**
 * @brief Destructor for DebugController
 */
DebugController::~DebugController() {
}

/**
 * @brief Same as _ready() in GDScript.
 */
void DebugController::_ready() {
	// Initialize the pointer towards the dictionary of properties
	properties = Debug::get_singleton()->get_debug_properties();
	// Initialize the pointer towards the debug menu container
	property_container = Node::cast_to<VBoxContainer>(find_child("VBoxContainer", true, false));
}

/**
 * @brief Same as _process() in GDScript.
 */
void DebugController::_process(double _delta) {
	// Grabs all the values from the debug properties dictionary and adds/updates them on the debug menu
	if (!properties.is_empty()) {
		for (int i = 0; i < properties.size(); i++) {
			String name = UtilityFunctions::str(properties.keys().operator[](i));
			String value = UtilityFunctions::str(properties.operator[](name));

			debugText = Node::cast_to<Label>(property_container->find_child(name, true, false));

			if (!debugText) {
				debugText = memnew(Label());

				debugText->set_name(name);
				debugText->set_text(name + ": " + value);
				debugText->add_theme_font_size_override("font_size", 24);
				property_container->add_child(debugText);
			} else {
				debugText->set_text(name + ": " + value);
			}
		}
	}
}

/**
 * @brief Same as _exit_tree() in GDScript.
 */
void DebugController::_exit_tree() {
	// Deletes all children of the debug menu upon exiting the scene tree
	for (int i = 0; i < property_container->get_child_count(); i++) {
		property_container->get_child(i)->queue_free();
		property_container->remove_child(property_container->get_child(i));
	}
}