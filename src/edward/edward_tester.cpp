// This class is a test bed for Edward's new code. 

#include "edward_tester.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

// Binding function for exposing methods/ properties to Godot. 
void EdwardTester::_bind_methods() {
}

// Constructor. 
EdwardTester::EdwardTester() {
}

// Destructor. 
EdwardTester::~EdwardTester() {
}

// Export instance variables to the Godot Editor. 
_GDEXPORT_ADD_PREFIX(EdwardTester)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_GET_PREFIX(EdwardTester)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_SET_PREFIX(EdwardTester)
_GDEXPORT_SET_SUFFIX

// Same as _process() in GDScript. 
void EdwardTester::_process(double delta) {
}