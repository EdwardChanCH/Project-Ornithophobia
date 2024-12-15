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
    ClassDB::bind_method(D_METHOD("import_scene_tscn", "filepath"), &SceneManager::import_scene_tscn);
    ClassDB::bind_method(D_METHOD("load_new_scene_node", "scene_tree", "filepath", "new_scene_node"), &SceneManager::load_new_scene_node);
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
 * @brief File import function for .tscn format.
 * Return nullptr if filepath does not exist.
 * 
 * @param filepath Filepath
 * @return Node* Scene Node
 */
Node * SceneManager::import_scene_tscn(String filepath) {
    if (ResourceLoader::get_singleton()->exists(filepath, "PackedScene")) {
        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        return scene.ptr()->instantiate();
    } else {
        return nullptr;
    }
}

/**
 * @brief Load a new scene node, and track the unloaded scene. 
 * Use import_scene_tscn() to get a new_scene_node. 
 * Note: Make all the function calls to new_scene_node BEFORE passing it into this function!
 * 
 * @param scene_tree Scene Tree
 * @param filepath Filepath
 * @param new_scene_node New Scene Node (can be nullptr)
 * @return true 
 * @return false 
 */
bool SceneManager::load_new_scene_node(SceneTree *scene_tree, String filepath, Node *new_scene_node) {
    try {
        // Validate input
        if (scene_tree == nullptr) {
            UtilityFunctions::print("Warning: Scene tree cannot be null.");
            return false;
        }

        // Store current scene in stack
        scene_stack->push_back(scene_tree->get_current_scene()->get_scene_file_path());

        // Switch to new scene using filepath (backward compatible)
        if (!new_scene_node) {
            scene_tree->change_scene_to_file(filepath);
            return true;
        }

        // Switch to new scene using node (to pass information into the next scene)
        Window *scene_root = scene_tree->get_root();
        Node *old_scene_node = scene_tree->get_current_scene();
        
        old_scene_node->queue_free();
        scene_root->remove_child(old_scene_node);
        scene_root->call_deferred("add_child", new_scene_node);
        scene_tree->call_deferred("set_current_scene", new_scene_node);
        //scene_root->add_child(new_scene_node);
        //scene_tree->set_current_scene(new_scene_node);

        return true;
        
    } catch (Error e) {
        UtilityFunctions::print("Warning: Failed to load scene from filepath.");
        return false;
    }
}

/**
 * @brief Load a new scene file, and track the unloaded scene. 
 * Note: Use load_new_scene_node() if the next scene needs data passed.
 * 
 * @param scene_tree Scene Tree
 * @param filepath Filepath
 * @return true 
 * @return false 
 */
bool SceneManager::load_new_scene(SceneTree *scene_tree, String filepath) {
    // For backward compatibility
    return load_new_scene_node(scene_tree, filepath, nullptr);
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