#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include "gameinput.h"
#include "gameaxis.h"
#include "globals.h"

namespace godot {

    class InputHandler : public Object{

        GDCLASS(InputHandler, Object)
        
        static InputHandler *singleton;
        
        protected:
            static void _bind_methods();
        
        private:
            InputHandler();
            ~InputHandler();

        public:
            InputHandler(InputHandler &other) = delete;

            GameInput *MOVE_LEFT;
            GameInput *MOVE_RIGHT;
            GameAxis *MOVE_HORIZONTAL;
            GameInput *SMALL_BLAST;
            GameInput *LARGE_BLAST;

            static InputHandler *get_singleton();
            bool is_move_input_pressed();
            void update_last_values();

    };

}

#endif