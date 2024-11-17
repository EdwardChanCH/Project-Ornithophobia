// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "inputhandler.h"

using namespace godot;

InputHandler *InputHandler::singleton = nullptr;

void InputHandler::_bind_methods() {
    ClassDB::bind_static_method("InputHandler", D_METHOD("get_singleton"), &InputHandler::get_singleton);
    ClassDB::bind_method(D_METHOD("is_move_input_pressed"), &InputHandler::is_move_input_pressed);
    ClassDB::bind_method(D_METHOD("update_last_values"), &InputHandler::update_last_values);
}

InputHandler::InputHandler() {
    MOVE_LEFT = memnew(GameInput("move_left"));
    MOVE_RIGHT = memnew(GameInput("move_right"));
    MOVE_HORIZONTAL = memnew(GameAxis("move_left", "move_right"));
    SMALL_BLAST = memnew(GameInput("small_blast"));
    LARGE_BLAST = memnew(GameInput("large_blast"));
}

InputHandler::~InputHandler() {
    if (singleton == this) {
        ClassDB::_unregister_engine_singleton(InputHandler::get_class_static());
        memdelete(singleton);
        singleton = nullptr;
    }

    memdelete(MOVE_LEFT);
    memdelete(MOVE_RIGHT);
    memdelete(MOVE_HORIZONTAL);
    memdelete(SMALL_BLAST);
    memdelete(LARGE_BLAST);
}

InputHandler *InputHandler::get_singleton() {
    if (singleton == nullptr) {
        singleton = memnew(InputHandler);
    } else {
        ClassDB::_register_engine_singleton(InputHandler::get_class_static(), singleton);
    }

    return singleton;
}

bool InputHandler::is_move_input_pressed() {
    UtilityFunctions::print("test");
    return MOVE_HORIZONTAL->get_value() != 0;
}

void InputHandler::update_last_values() {
    MOVE_LEFT->update_last();
    MOVE_RIGHT->update_last();
    MOVE_HORIZONTAL->update_last();
    SMALL_BLAST->update_last();
    LARGE_BLAST->update_last();
}
