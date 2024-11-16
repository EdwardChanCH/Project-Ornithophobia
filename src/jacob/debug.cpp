// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "debug.h"

using namespace godot;

Debug *Debug::singleton = nullptr;

void Debug::_bind_methods() {
    ClassDB::bind_static_method("Debug", D_METHOD("get_singleton"), &Debug::get_singleton);
    ClassDB::bind_method(D_METHOD("add_debug_property", "name", "value", "order"), &Debug::add_debug_property);
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
    memdelete(debugProperties);
}

void Debug::add_debug_property(String name, String value, int order) {
    // Label *target;

    UtilityFunctions::print(debugProperties);

    // target.set_name(name);
    // target.set_text(name + ": " + value);
}