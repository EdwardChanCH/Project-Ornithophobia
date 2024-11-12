#ifndef GAMEAXIS_H
#define GAMEAXIS_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

    class GameAxis : public Object {

        GDCLASS(GameAxis, Object)

        protected:
            static void _bind_methods();

        public:
            String positiveId;
            String negativeId;

            GameAxis();
            GameAxis(String positiveId, String negativeId);
            ~GameAxis();

            float getValue() const;
            float getLastValue() const;
            void updateLast();

        private:
            float lastValue = 0;
    };

}

#endif