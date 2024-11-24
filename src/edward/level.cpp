/**
 * @file level.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Prototype Pattern + Mediator Pattern (MVC).
 * This class handles all data storage of a level.
 * Custom user levels are cloned from a default level object instead of constructed from scratch.
 * @version 0.2.0
 * @date 2024-11-23
 * 
 */

#include "level.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(Level)
_GDEXPORT_ADD(PropertyInfo(Variant::DICTIONARY, "level_info"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(Level)
_GDEXPORT_GET(level_info)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(Level)
_GDEXPORT_SET(level_info)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void Level::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clone"), &Level::clone);
    ClassDB::bind_static_method("Level", D_METHOD("import_level_tscn", "filepath"), &Level::import_level_tscn);
    ClassDB::bind_static_method("Level", D_METHOD("export_level_tscn", "filepath", "level_node"), &Level::export_level_tscn);
    ClassDB::bind_method(D_METHOD("get_level_info"), &Level::get_level_info);
    ClassDB::bind_method(D_METHOD("set_level_info", "value"), &Level::set_level_info);
    ClassDB::bind_method(D_METHOD("get_list", "list_name"), &Level::get_list);
    ClassDB::bind_method(D_METHOD("clear_list", "list_name"), &Level::clear_list);
    ClassDB::bind_method(D_METHOD("add_node_to_list", "list_name", "new_node"), &Level::add_node_to_list);
}

/**
 * @brief Construct a new Level:: Level object.
 * 
 */
Level::Level() {
    level_info = Dictionary();
}

/**
 * @brief Deep copy of a Level:: Level object.
 * Note: Use clone() instead to also duplicate its children.
 * 
 */
Level::Level(Level * level) {
    this->level_info = level->level_info.duplicate(true);
}

/**
 * @brief Destroy the Level:: Level object.
 * 
 */
Level::~Level() {
}

/**
 * @brief Clones a level node and all of its children.
 * 
 * @return Level* Cloned Level
 */
Level * Level::clone() {
    int32_t flag = Node::DUPLICATE_SIGNALS + Node::DUPLICATE_USE_INSTANTIATION;
    Level * clone_level = (Level *)this->duplicate(flag);
    clone_level->level_info = this->level_info.duplicate(true);
    return clone_level;
}

/**
 * @brief File import function for .tscn format.
 * Warning: The input is not validated.
 * 
 * @param filepath Filepath
 * @return Level* Level Node
 */
Level * Level::import_level_tscn(String filepath) {
    Ref<PackedScene> level_scene = ResourceLoader::get_singleton()->load(filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
    return (Level *)level_scene.ptr()->instantiate();
}

/**
 * @brief File export function for .tscn format.
 * Warning: The input is not validated.
 * 
 * @param filepath Filepath
 * @param level_node Level Node
 */
void Level::export_level_tscn(String filepath, Level * level_node) {
    Ref<PackedScene> level_scene;
    level_scene.reference_ptr(memnew(PackedScene));
    level_scene.ptr()->pack(level_node);
    ResourceSaver::get_singleton()->save(level_scene, filepath, 0);
}

/**
 * @brief Get level info dictionary.
 * 
 * @return Dictionary Level Info
 */
Dictionary Level::get_level_info() {
    Variant value;
    _get("level_info", value);
    return (Dictionary)value;
}

/**
 * @brief Set level info dictionary.
 * 
 * @param value New Dictionary
 */
void Level::set_level_info(Dictionary value) {
    _set("level_info", value);
}

/**
 * @brief Get the list.
 * Make the list if the list is missing.
 * Note: Use add_node_to_list() to add child node to this list safely!
 * 
 * @param list_name List Name (constant)
 * @return Node* List Node
 */
Node * Level::get_list(String list_name) {
    Node * list;

    if (has_node(list_name)) {
        list = get_node_or_null(list_name);
    } else {
        list = memnew(Node);
        list->set_name(list_name);
        add_child(list, true);
        list->set_owner(this);
    }

    return list;
}

/**
 * @brief Free all children nodes in the list.
 * 
 * @param list_name List Name (constant)
 */
void Level::clear_list(String list_name) {
    Node * list = get_list(list_name);
    int count = list->get_child_count(); 

    for (int32_t i = 0; i < count; ++i) {
        list->get_child(i)->queue_free();
    }
}

/**
 * @brief Add a node to a list.
 * Make the list if the list is missing.
 * Note: The player list and tile list can only hold 1 child.
 * 
 * @param list_name List Name (constant)
 * @param new_node New Node
 * @return true 
 * @return false 
 */
bool Level::add_node_to_list(String list_name, Node * new_node) {
    if (!new_node) {
        return false;
    }

    if ((list_name.nocasecmp_to(player_list_name) == 0) || (list_name.nocasecmp_to(tile_list_name) == 0)) {
        clear_list(list_name);
    }
    
    get_list(list_name)->add_child(new_node);
    new_node->set_owner(this);

    return true;
}