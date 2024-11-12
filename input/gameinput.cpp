// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "gameinput.h"
#include "globals.h"

using namespace godot;

void GameInput::_bind_methods() {
}

// GameInput::GameInput() {
// }

GameInput::GameInput(String id) {
    GameInput::id = id;
    WARN_PRINT("test");
    UtilityFunctions::print("test");
}

GameInput::~GameInput() {
    
}

bool GameInput::isJustPressed() const {
    return Input::get_singleton() -> is_action_just_pressed(id);
}

bool GameInput::isJustReleased() const {
    return Input::get_singleton() -> is_action_just_released(id);
}

bool GameInput::wasPressed() const {
    return lastValue;
}

bool GameInput::isPressed() const {
    return Input::get_singleton() -> is_action_pressed(id);
}

void GameInput::updateLast() {
    lastValue = isPressed();
}