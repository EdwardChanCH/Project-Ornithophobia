/**
 * @file scene_manager.h
 * @author
 * @brief [Singleton --- Creational Design Patterns]
 * This class solely manages the transition between scenes.
 * @version
 * @date
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
		TypedArray<String> * sceneStack;

		SceneManager(); // Private Constructor
		~SceneManager(); // Private Destructor

	protected:
		static void _bind_methods(); // Must be declared

	public:
		SceneManager(SceneManager &other) = delete; // No copying
		// void operator=(const SceneManager &) = delete; // No assigning (keep this for Godot to work)
		
		static SceneManager * get_instance();
		bool load_new_scene(SceneTree &sceneTree, const String &filepath);
		bool load_previous_scene(SceneTree &sceneTree);
		void debug();
	};

} // namespace godot

#endif