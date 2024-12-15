/**
 * @file scene_manager.h
 * @author Chun Ho Chan (Edward)
 * @brief Header file.
 * @version 0.2.0
 * @date 2024-11-16
 * 
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/object.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

namespace godot {

	class SceneManager : public Object {
		
		GDCLASS(SceneManager, Object) // A Godot macro for class inheritance

	private:
		static SceneManager * singleton;
		TypedArray<String> * scene_stack;

		SceneManager(); // Private Constructor
		~SceneManager(); // Private Destructor

	protected:
		static void _bind_methods(); // Must be declared

	public:
		SceneManager(SceneManager &other) = delete; // No copying
		// void operator=(const SceneManager &) = delete; // No assigning (keep this for Godot to work)
		
		static SceneManager * get_instance();
		Node * import_scene_tscn(String filepath);
		bool load_new_scene_node(SceneTree *scene_tree, String filepath, Node *new_scene_node);
		bool load_new_scene(SceneTree *scene_tree, String filepath);
		bool load_previous_scene(SceneTree *scene_tree);
		void _debug();
	};

} // namespace godot

#endif