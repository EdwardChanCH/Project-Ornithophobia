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
    
    private:
        Dictionary *debugProperties;

        Debug();
        ~Debug();

    public:
        Debug(Debug &other) = delete;

        static Debug *get_singleton();
        void add_debug_property(String name, Variant value);
        Dictionary *get_debug_properties();
    };

}

#endif