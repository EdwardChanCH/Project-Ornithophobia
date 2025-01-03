/**
 * @file level.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.3.0
 * @date 2024-11-25
 * 
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes
#include "iprototype.h"

namespace godot {

	class Level : public IPrototype {
		
		GDCLASS(Level, IPrototype) // A Godot macro for class inheritance
		_GDEXPORT // A custom macro for exporting instance variables

	private:
		Dictionary level_info; // Dictionary of any text/ numerical level data

	protected:
		static void _bind_methods(); // Must be declared

	public:
		Level();
		Level(Level * level);
		~Level();

		virtual Level * clone() override;

		static Level * import_level_tscn(String filepath);
		static void export_level_tscn(String filepath, Level * level_node);

		Dictionary get_level_info();
		void set_level_info(Dictionary value);

		void set_physics(bool active);

		Node * get_list(String list_name);
		bool add_list(String list_name);
		void clear_list(String list_name);
		int get_list_length(String list_name);

		Node * get_node_in_list(String list_name, String node_name);
		Node * get_node_in_list_by_index(String list_name, int index);
		bool add_node_to_list(String list_name, Node * child_node);

		// List name constants
		const String tile_list_name = "TileList";
		const String player_list_name = "PlayerList";
		const String enemy_list_name = "EnemyList";
		const String entity_list_name = "EntityList";
	};

} // namespace godot

#endif