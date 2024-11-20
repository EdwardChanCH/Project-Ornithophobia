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

            GameInput();
            GameInput(String id);
            ~GameInput();

            bool is_just_pressed() const;
            bool is_just_released() const;
            bool was_pressed() const;
            bool is_pressed() const;
            void update_last();

        private:
            bool lastValue = false;
    };

}

#endif