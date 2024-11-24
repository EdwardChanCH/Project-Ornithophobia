/**
 * @file iprototype.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Prototype Pattern.
 * Abstract interface for clonable classes.
 * @version 0.1.0
 * @date 2024-11-13
 * 
 */

#include "iprototype.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(IPrototype)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(IPrototype)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(IPrototype)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void IPrototype::_bind_methods() {
    BIND_VIRTUAL_METHOD(IPrototype, clone)
}
