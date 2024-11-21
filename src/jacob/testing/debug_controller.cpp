// This example class moves an image sprite around in a scene.

#include "debug_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void DebugController::_bind_methods() {
}

// Constructor. 
DebugController::DebugController() {
}

// Destructor. 
DebugController::~DebugController() {
}

void DebugController::_ready() {
	properties = Debug::get_singleton()->get_debug_properties();
	property_container = Node::cast_to<VBoxContainer>(find_child("VBoxContainer", true, false));
	
	for (int i = 0; i < property_container->get_child_count(); i++) {
		property_container->get_child(i)->queue_free();
		property_container->remove_child(property_container->get_child(i));
	}
	
	UtilityFunctions::print("tree");
}

void DebugController::_process(double _delta) {
	if (properties != nullptr) {
		for (int i = 0; i < properties->size(); i++) {
			String name = UtilityFunctions::str(properties->keys().operator[](i));
			String value = UtilityFunctions::str(properties->operator[](name));

			debugText = Node::cast_to<Label>(property_container->find_child(name, true, false));

			if (!debugText) {
				debugText = memnew(Label());

				debugText->set_name(name);
				debugText->set_text(name + ": " + value);
				property_container->add_child(debugText);
			} else {
				debugText->set_text(name + ": " + value);
			}
		}
	}
	
}

void DebugController::_exit_tree() {
	for (int i = 0; i < property_container->get_child_count(); i++) {
		property_container->get_child(i)->queue_free();
		property_container->remove_child(property_container->get_child(i));
	}
}