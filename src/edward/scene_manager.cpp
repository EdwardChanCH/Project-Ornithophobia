/**
 * @file scene_manager.cpp
 * @author
 * @brief [Singleton Pattern --- Creational Design Patterns]
 * This class solely manages the transition between scenes in Godot.
 * @version 0.1.0
 * @date 2024-11-16
 * 
 */

#include "scene_manager.h"
#include <iostream>

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
    //ClassDB::bind_method(D_METHOD("load_new_scene", "scene_tree", "filepath"), &SceneManager::load_new_scene);
    //ClassDB::bind_method(D_METHOD("load_previous_scene", "scene_tree"), &SceneManager::load_previous_scene);
    ClassDB::bind_method(D_METHOD("debug"), &SceneManager::debug);
}

/**
 * @brief Construct a new Scene Manager:: Scene Manager object.
 * 
 */
SceneManager::SceneManager() {
    sceneStack = memnew(TypedArray<String>);
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

    memdelete(sceneStack);
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

bool SceneManager::load_new_scene(SceneTree &sceneTree, const String &filepath) {
    try {
        sceneStack->push_back(filepath);
        sceneTree.change_scene_to_file(filepath);
        return true;
    } catch (Error e) {
        UtilityFunctions::print("Error: Failed to load new scene from filepath.");
        return false;
    }
}

bool SceneManager::load_previous_scene(SceneTree &sceneTree) {
    try {
        if (sceneStack->size() <= 0) {
            UtilityFunctions::print("Warning: No previous scene in stack.");
            return false;
        }
        String filepath = sceneStack->pop_back();
        sceneTree.change_scene_to_file(filepath);
        return true;
    } catch (Error e) {
        UtilityFunctions::print("Error: Failed to load previous scene from filepath.");
        return false;
    }
}

void SceneManager::debug() {
    UtilityFunctions::print("> Debugging SceneManager ... v1");
    // for (int i = 0; i < sceneStack->size(); ++i) {
    //     UtilityFunctions::print(sceneStack[i]);
    // }
    auto p = get_instance();

    UtilityFunctions::print("> End of debugging.");
}