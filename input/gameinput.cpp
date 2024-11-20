// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "gameinput.h"
#include "globals.h"

using namespace godot;

void GameInput::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_just_pressed"), &GameInput::is_just_pressed);
    ClassDB::bind_method(D_METHOD("is_just_released"), &GameInput::is_just_released);
    ClassDB::bind_method(D_METHOD("was_pressed"), &GameInput::was_pressed);
    ClassDB::bind_method(D_METHOD("is_pressed"), &GameInput::is_pressed);
    ClassDB::bind_method(D_METHOD("update_last"), &GameInput::update_last);
}

GameInput::GameInput() {
}

GameInput::GameInput(String id) {
    GameInput::id = id;
}

GameInput::~GameInput() {
    
}

bool GameInput::is_just_pressed() const {
    return Input::get_singleton()->is_action_just_pressed(id);
}

bool GameInput::is_just_released() const {
    return Input::get_singleton()->is_action_just_released(id);
}

bool GameInput::was_pressed() const {
    return lastValue;
}

bool GameInput::is_pressed() const {
    return Input::get_singleton()->is_action_pressed(id);
}

void GameInput::update_last() {
    lastValue = is_pressed();
}