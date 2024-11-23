/**
 * @file iprototype.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Prototype Pattern.
 * Abstract interface class.
 * @version 0.1.0
 * @date 2024-11-13
 * 
 */

#include "iprototype.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void IPrototype::_bind_methods() {
    BIND_VIRTUAL_METHOD(IPrototype, clone)
}
