// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "inputhandler.h"

using namespace godot;

bool InputHandler::isMoveInputPressed() {
    UtilityFunctions::print("test");
    return MOVE_HORIZONTAL.getValue() != 0;
}

void InputHandler::updateLastValues() {
    MOVE_LEFT.updateLast();
    MOVE_RIGHT.updateLast();
    MOVE_HORIZONTAL.updateLast();
    SMALL_BLAST.updateLast();
    LARGE_BLAST.updateLast();
}
