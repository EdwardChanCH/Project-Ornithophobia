#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

    class GameInput : public Object {

        GDCLASS(GameInput, Object)

        protected:
            static void _bind_methods();

        public:
            String id;

            // GameInput();
            GameInput(String id);
            ~GameInput();

            bool isJustPressed() const;
            bool isJustReleased() const;
            bool wasPressed() const;
            bool isPressed() const;
            void updateLast();

        private:
            bool lastValue = false;
    };

}

#endif