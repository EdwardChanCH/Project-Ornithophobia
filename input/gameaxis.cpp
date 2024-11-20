#include "gameaxis.h"

using namespace godot;

void GameAxis::_bind_methods() {
}

GameAxis::GameAxis() {
}

GameAxis::GameAxis(String positiveId, String negativeId) {
    GameAxis::positiveId = positiveId;
    GameAxis::negativeId = negativeId;
}

GameAxis::~GameAxis() {

}

float GameAxis::get_value() const {
    return Input::get_singleton()->get_axis(positiveId, negativeId);
}

float GameAxis::get_last_value() const {
    return lastValue;
}

void GameAxis::update_last() {
    lastValue = get_last_value();
}