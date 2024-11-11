// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "timecontroller.h"

using namespace godot;

void TimeController::_bind_methods() {
}

TimeController::TimeController() {
}

TimeController::~TimeController() {
}

TimeController* timeController = nullptr;

void TimeController::set_game_speed(float speed) {
    Engine::get_singleton()->set_time_scale(speed);
}
