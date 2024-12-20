/**
 * @file debug.cpp
 * @author Jacob Couture
 * @brief This class is a singleton that holds a dictionary of properties that should appear on the debug menu in-game.
 */

#include "debug.h"

using namespace godot;

// Pointer to the instance of this singleton
Debug *Debug::singleton = nullptr;

/**
 * @brief Binding function for exposing methods/properties to Godot.
 */
void Debug::_bind_methods() {
    ClassDB::bind_static_method("Debug", D_METHOD("get_singleton"), &Debug::get_singleton);
    ClassDB::bind_method(D_METHOD("add_debug_property", "name", "value"), &Debug::add_debug_property);
    //TODO: Make this bind properly
    ClassDB::bind_method(D_METHOD("get_debug_properties"), &Debug::get_debug_properties);
    ClassDB::bind_method(D_METHOD("is_debug_mode_active"), &Debug::is_debug_mode_active);
    ClassDB::bind_method(D_METHOD("set_debug_mode", "mode"), &Debug::set_debug_mode);
}

/**
 * @brief Constructor for the Debug class
 */
Debug::Debug() {
    debugProperties = Dictionary();
    isDebugModeActive = false;
}

/**
 * @brief Destructor for the Debug class
 */
Debug::~Debug() {
    if (singleton == this) {
        ClassDB::_unregister_engine_singleton(Debug::get_class_static());
        memdelete(singleton);
        singleton = nullptr;
    }
}

/**
 * @brief Gets the instance of this singleton.
 */
Debug *Debug::get_singleton() {
    if (singleton == nullptr) {
        singleton = memnew(Debug);
    } else {
        ClassDB::_register_engine_singleton(Debug::get_class_static(), singleton);
    }

    return singleton;
}

/**
 * @brief Registers a new property to track in the debug menu
 * @param name The name of the property to be tracked
 * @param value The value of the property to be tracked
 */
void Debug::add_debug_property(String name, Variant value) {
    if (!debugProperties.has(name)) {
        debugProperties.get_or_add(name, value);
    } else {
        debugProperties.operator[](name) = value;
    }
}

/**
 * @brief Getter for the dictionary of tracked debug properties
 * @return The dictionary containing all tracked debug properties
 */
Dictionary Debug::get_debug_properties() {
    return debugProperties;
}


/**
 * @brief Getter for the isDebugModeActive flag
 * @return The value of isDebugModeActive
 */
bool Debug::is_debug_mode_active() {
    return isDebugModeActive;
}


/**
 * @brief Setter for the isDebugModeActive variable
 * @param mode The new value for isDebugModeActive
 */
void Debug::set_debug_mode(bool mode) {
    isDebugModeActive = mode;
}