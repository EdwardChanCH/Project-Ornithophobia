#ifndef EDWARD_TESTER_H
#define EDWARD_TESTER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes. 
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class. 
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code. 

namespace godot {

	class EdwardTester : public Node {
		
		GDCLASS(EdwardTester, Node) // A Godot macro for class inheritance. 
		_GDEXPORT // A custom macro for exporting instance variables. 

	private:

	protected:
		static void _bind_methods(); // Must be declared. 

	public:
		EdwardTester();
		~EdwardTester();

		void _process(double delta) override;
	};

} // namespace godot

#endif