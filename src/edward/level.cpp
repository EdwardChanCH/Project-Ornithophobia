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
    //ClassDB::bind_method(D_METHOD("clone"), &Level::clone);
    ClassDB::bind_static_method("Level", D_METHOD("import_level_tscn", "filepath"), &Level::import_level_tscn);
    ClassDB::bind_static_method("Level", D_METHOD("export_level_tscn", "filepath", "level_node"), &Level::export_level_tscn);
    ClassDB::bind_method(D_METHOD("get_level_info"), &Level::get_level_info);
    ClassDB::bind_method(D_METHOD("set_level_info", "value"), &Level::set_level_info);
    ClassDB::bind_method(D_METHOD("get_list", "list_name"), &Level::get_list);
    ClassDB::bind_method(D_METHOD("add_list", "list_name"), &Level::get_list);
    ClassDB::bind_method(D_METHOD("clear_list", "list_name"), &Level::clear_list);
    ClassDB::bind_method(D_METHOD("get_node_in_list", "list_name", "node_name"), &Level::get_node_in_list);
    ClassDB::bind_method(D_METHOD("add_node", "parent_node", "child_node"), &Level::add_node);
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
 * @brief Called when this node is ready in Godot's scene tree.
 * 
 */
void Level::_ready() {
    add_list(tile_list_name);
    add_list(player_list_name);
    add_list(enemy_list_name);
    add_list(entity_list_name);
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
    return (Level *)(level_scene.ptr()->instantiate());
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
 * Return nullptr if list does not exist.
 * Note: Use add_node() to add child node to this list safely!
 * 
 * @param list_name List Name (constant)
 * @return Node* List Node
 */
Node * Level::get_list(String list_name) {
    if (has_node(list_name)) {
        return get_node_or_null(list_name);
    } else {
        return nullptr;
    }
}

void Level::add_list(String list_name) {
    Node * list;

    if (has_node(list_name)) {
        return;
    }

    list = memnew(Node);
    list->set_name(list_name);
    add_node(this, list);
}

/**
 * @brief Free all children nodes in the list.
 * 
 * @param list_name List Name (constant)
 */
void Level::clear_list(String list_name) {
    Node * list, * child;
    
    list = get_list(list_name);
    if (!list) {
        return;
    }

    while (list->get_child_count() > 0) {
        child = list->get_child(0);
        list->remove_child(child);
        child->queue_free();
    }
}

/**
 * @brief Get the node in the list.
 * Return nullptr if list or node does not exist.
 * 
 * @param list_name List Name (constant)
 * @param node_name Node Name
 * @return Node* List Node
 */
Node * Level::get_node_in_list(String list_name, String node_name) {
    Node * list, * node;

    list = get_list(list_name);
    if (!list) {
        return nullptr;
    }

    if (list->has_node(node_name)) {
        return list->get_node_or_null(NodePath(node_name));
    } else {
        return nullptr;
    }
}

/**
 * @brief Add a node to a list.
 * Return false if list or node is missing.
 * 
 * @param list_name List Name (constant)
 * @param new_node New Node
 * @return true 
 * @return false 
 */
bool Level::add_node(Node * parent_node, Node * child_node) {
    if ((!parent_node) || (!child_node)) {
        return false;
    }
    
    parent_node->add_child(child_node);
    child_node->set_owner(this);
    return true;
}