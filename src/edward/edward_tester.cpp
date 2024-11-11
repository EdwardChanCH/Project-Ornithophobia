/**
 * @file edward_tester.cpp
 * @author Chun Ho Chan
 * @brief This class is a test bed for Edward's new code.
 * @version 0.1.0
 * @date 2024-11-10
 * 
 */

#include "edward_tester.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void EdwardTester::_bind_methods() {
}

/**
 * @brief Construct a new Edward Tester:: Edward Tester object.
 * 
 */
EdwardTester::EdwardTester() {
}

/**
 * @brief Destroy the Edward Tester:: Edward Tester object.
 * 
 */
EdwardTester::~EdwardTester() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(EdwardTester)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(EdwardTester)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(EdwardTester)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void EdwardTester::_process(double delta) {
}