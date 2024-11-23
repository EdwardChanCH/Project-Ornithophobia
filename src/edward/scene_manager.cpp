/**
 * @file scene_manager.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Singleton Pattern.
 * This class solely manages the transition between scenes in Godot.
 * @version 0.2.0
 * @date 2024-11-16
 * 
 */

#include "scene_manager.h"

namespace godot {
    // Define static variables
    SceneManager * SceneManager::singleton = nullptr;

} // namespace godot 

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void SceneManager::_bind_methods() {
    ClassDB::bind_static_method("SceneManager", D_METHOD("get_instance"), &SceneManager::get_instance);
    ClassDB::bind_method(D_METHOD("load_new_packed_scene", "scene_tree", "packed_scene"), &SceneManager::load_new_packed_scene);
    ClassDB::bind_method(D_METHOD("load_new_scene", "scene_tree", "filepath"), &SceneManager::load_new_scene);
    ClassDB::bind_method(D_METHOD("load_previous_scene", "scene_tree"), &SceneManager::load_previous_scene);
    ClassDB::bind_method(D_METHOD("_debug"), &SceneManager::_debug);
}

/**
 * @brief Construct a new Scene Manager:: Scene Manager object.
 * 
 */
SceneManager::SceneManager() {
    scene_stack = memnew(TypedArray<String>);
}

/**
 * @brief Destroy the Scene Manager:: Scene Manager object.
 * 
 */
SceneManager::~SceneManager() {
    if (singleton == this) {
        ClassDB::_unregister_engine_singleton(SceneManager::get_class_static());
        memdelete(singleton);
        singleton = nullptr;
    }

    memdelete(scene_stack);
    scene_stack = nullptr;
}

/**
 * @brief Get the singleton instance of this class.
 * 
 * @return SceneManager* 
 */
SceneManager * SceneManager::get_instance() {
    if (singleton == nullptr) {
        singleton = memnew(SceneManager);
    }

    if (singleton != nullptr) {
        ClassDB::_register_engine_singleton(SceneManager::get_class_static(), singleton);
    }

    return singleton;
}

/**
 * @brief Load a new packed scene, and track the unloaded scene. 
 * Useful for passing data to the next scene.
 * 
 * @param scene_tree Scene Tree
 * @param packed_scene Packed Scene (initialised)
 * @return true 
 * @return false 
 */
bool SceneManager::load_new_packed_scene(SceneTree *scene_tree, Ref<PackedScene> packed_scene) {
    try {
        // Validate input
        if (scene_tree == nullptr) {
            UtilityFunctions::print("Warning: Scene tree cannot be null.");
            return false;
        }

        // Store current scene in stack
        scene_stack->push_back(scene_tree->get_current_scene()->get_scene_file_path());

        // Switch to new scene
        scene_tree->change_scene_to_packed(packed_scene);
        return true;

    } catch (Error e) {
        UtilityFunctions::print("Warning: Failed to load packed scene.");
        return false;
    }
}

/**
 * @brief Load a new scene file, and track the unloaded scene. 
 * Note: Use load_new_packed_scene() if the next scene needs data passed.
 * 
 * @param scene_tree Scene Tree
 * @param filepath Filepath
 * @return true 
 * @return false 
 */
bool SceneManager::load_new_scene(SceneTree *scene_tree, String filepath) {
    try {
        // Validate input
        if (scene_tree == nullptr) {
            UtilityFunctions::print("Warning: Scene tree cannot be null.");
            return false;
        }

        // Store current scene in stack
        scene_stack->push_back(scene_tree->get_current_scene()->get_scene_file_path());

        // Switch to new scene
        scene_tree->change_scene_to_file(filepath);
        return true;

    } catch (Error e) {
        UtilityFunctions::print("Warning: Failed to load scene from filepath.");
        return false;
    }
}

/**
 * @brief Load the previous scene, and unload the current scene.
 * 
 * @param scene_tree Scene Tree
 * @return true 
 * @return false 
 */
bool SceneManager::load_previous_scene(SceneTree *scene_tree) {
    String filepath;

    try {
        // Validate input
        if (scene_tree == nullptr) {
            UtilityFunctions::print("Warning: Scene Tree cannot be null.");
            return false;
        }

        if (scene_stack->size() > 0) {
            // Pop previous scene
            filepath = scene_stack->pop_back();
        } else {
            // No previous scene
            UtilityFunctions::print("Warning: No previous scene!");
            return false;
        }

        // Switch to previus scene
        scene_tree->change_scene_to_file(filepath);
        return true;

    } catch (Error e) {
        UtilityFunctions::print("Warning: Failed to load scene from filepath.");
        return false;
    }
}

/**
 * @brief Debug function.
 * 
 */
void SceneManager::_debug() {
    UtilityFunctions::print("------------------------------");

    UtilityFunctions::print("Class: SceneManager");

    UtilityFunctions::print("Scene Stack:");
    for (int i = 0; i < scene_stack->size(); ++i)
        UtilityFunctions::print((*scene_stack)[i]);

    UtilityFunctions::print("------------------------------");
}