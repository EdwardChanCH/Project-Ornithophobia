// This example class moves an image sprite around in a scene.

#include "debug_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void DebugController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_debug_property"), &DebugController::add_debug_property);
}

// Constructor. 
DebugController::DebugController() {
}

// Destructor. 
DebugController::~DebugController() {
	// memfree(debugText);
}

void DebugController::_ready() {
	property_container = Node::cast_to<VBoxContainer>(find_child("VBoxContainer", true, false));
	
	for (int i = 0; i < property_container->get_child_count(); i++) {
		property_container->get_child(i)->queue_free();
		property_container->remove_child(property_container->get_child(i));
	}
}

void DebugController::add_debug_property(String name, Variant value) {
    debugText = Node::cast_to<Label>(property_container->find_child(name, true, false));

	if (!debugText) {
		debugText = memnew(Label());

		debugText->set_name(name);
    	debugText->set_text(name + ": " + UtilityFunctions::str(value));
		property_container->add_child(debugText);
	} else {
		debugText->set_text(name + ": " + UtilityFunctions::str(value));
	}
}