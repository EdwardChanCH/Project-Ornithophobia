#ifndef GDEXAMPLE_2_NEW_H
#define GDEXAMPLE_2_NEW_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include <godot_cpp/classes/label.hpp> // Bindings to the parent class. 

namespace godot {

	class GDExample2New : public Label {
        GDCLASS(GDExample2New, Label)

    private:
        int counter;

    protected:
        static void _bind_methods(); // Must be declared. 

    public:
        GDExample2New();
		~GDExample2New();

        void print_received_signal(Vector2 p_position);
    };

} // namespace godot

#endif