#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include "gameinput.h"
#include "gameaxis.h"
#include "globals.h"

namespace godot {

    class InputHandler {
        public:
            InputHandler() = delete;

            static inline GameInput MOVE_LEFT = GameInput("move_left");
            static inline GameInput MOVE_RIGHT = GameInput("move_right");
            static inline GameAxis MOVE_HORIZONTAL = GameAxis("move_left", "move_right");
            static inline GameInput SMALL_BLAST = GameInput("small_blast");
            static inline GameInput LARGE_BLAST = GameInput("large_blast");

            static bool isMoveInputPressed();
            static void updateLastValues();

    };

}

#endif