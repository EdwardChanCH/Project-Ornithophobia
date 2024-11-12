#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code.
#include "globals.h"
// #include "inputhandler.h"

namespace godot {

    class TimeController : public Node {
        
        GDCLASS(TimeController, Node)

    private:


    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        TimeController();
        ~TimeController();

        void set_game_speed(float speed);

    };

}

#endif