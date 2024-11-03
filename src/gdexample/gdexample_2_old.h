#ifndef GDEXAMPLE_2_OLD_H
#define GDEXAMPLE_2_OLD_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include <godot_cpp/classes/label.hpp> // Bindings to the parent class. 

namespace godot {

	class GDExample2Old : public Label {
        GDCLASS(GDExample2Old, Label)

    private:
        int counter;

    protected:
		static void _bind_methods(); // Must be declared. 

    public:
        GDExample2Old();
		~GDExample2Old();

        void print_received_signal(Vector2 p_position);

        void _on_gd_example_1_old_position_changed_signal(Vector2 p_position);
    };

} // namespace godot

#endif