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

float GameAxis::getValue() const {
    return Input::get_singleton() -> get_axis(positiveId, negativeId);
}

float GameAxis::getLastValue() const {
    return lastValue;
}

void GameAxis::updateLast() {
    lastValue = getValue();
}