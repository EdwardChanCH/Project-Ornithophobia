#ifndef DEBUG_H
#define DEBUG_H

#include <godot_cpp/core/class_db.hpp>
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code.
#include "globals.h"
// #include "inputhandler.h"

namespace godot {

    class Debug : public Object {
        
        GDCLASS(Debug, Object)
        
        static Debug *singleton;

    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        static Debug *get_singleton();

        Dictionary *debugProperties = memnew(Dictionary());

        Debug();
        ~Debug();

        void add_debug_property(String name, String value, int order);
    };

}

#endif