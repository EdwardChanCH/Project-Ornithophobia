// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "debug.h"

using namespace godot;

Debug *Debug::singleton = nullptr;

void Debug::_bind_methods() {
    ClassDB::bind_static_method("Debug", D_METHOD("get_singleton"), &Debug::get_singleton);
    ClassDB::bind_method(D_METHOD("add_debug_property", "name", "value"), &Debug::add_debug_property);
}

Debug::Debug() {
    debugProperties = memnew(Dictionary());
}

Debug::~Debug() {
    if (singleton == this) {
        ClassDB::_unregister_engine_singleton(Debug::get_class_static());
        memdelete(singleton);
        singleton = nullptr;
    }

    memdelete(debugProperties);
}

Debug *Debug::get_singleton() {
    if (singleton == nullptr) {
        singleton = memnew(Debug);
    } else {
        ClassDB::_register_engine_singleton(Debug::get_class_static(), singleton);
    }

    return singleton;
}

void Debug::add_debug_property(String name, Variant value) {
    // UtilityFunctions::print(debugProperties);
    if (!debugProperties->has(name)) {
        debugProperties->get_or_add(name, value);
        UtilityFunctions::print(name);
    } else {
        debugProperties->operator[](name) = value;
    }
}

Dictionary *Debug::get_debug_properties() {
    return debugProperties;
}