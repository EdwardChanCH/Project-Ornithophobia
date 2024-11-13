// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "debug.h"

using namespace godot;

Debug *Debug::singleton = nullptr;

void Debug::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_debug_property"), &Debug::add_debug_property);
}

Debug *Debug::get_singleton() {
    return singleton;
}

Debug::Debug() {
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
}

Debug::~Debug() {
    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}

void Debug::add_debug_property(String name, String value, int order) {
    Label *target;

    // UtilityFunctions::print("working!!!!");

    // target.set_name(name);
    // target.set_text(name + ": " + value);
}