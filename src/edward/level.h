/**
 * @file level.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.1.0
 * @date 2024-11-13
 * 
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "iprototype.h" // Interface for clonable objects

namespace godot {

	class Level : public IPrototype {
		
		GDCLASS(Level, IPrototype) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		Dictionary level_stats; // Any non-node level data
		Node * player_list_node;
		Node * entity_list_node;
		Node * ally_list_node;
		Node * enemy_list_node;

	protected:
		static void _bind_methods(); // Must be declared

	public:
		const String player_list_node_name = "PlayerList";
		const String entity_list_node_name = "EntityList";
		const String ally_list_node_name = "AllyList";
		const String enemy_list_node_name = "EnemyList";

		Level();
		~Level();

		void _ready();

		virtual Level * clone() override;

		Node * setup_list_node(String list_node_name);
		bool add_node(Node * new_child_node, Node * parent_node);
	};

} // namespace godot

#endif