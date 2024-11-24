/**
 * @file level.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Prototype Pattern + Mediator Pattern (MVC).
 * This class handles all data storage of a level.
 * Custom user levels are cloned from a default level object instead of constructed.
 * @version 0.1.0
 * @date 2024-11-23
 * 
 */

#include "level.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(Level)
_GDEXPORT_ADD(PropertyInfo(Variant::DICTIONARY, "level_stats"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(Level)
_GDEXPORT_GET(level_stats)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(Level)
_GDEXPORT_SET(level_stats)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void Level::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clone"), &Level::clone);
}

/**
 * @brief Construct a new Level:: Level object.
 * 
 */
Level::Level() {
    level_stats = Dictionary();
    player_list_node = nullptr;
    entity_list_node = nullptr;
    ally_list_node = nullptr;
    enemy_list_node = nullptr;
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
    player_list_node = setup_list_node(player_list_node_name);
    entity_list_node = setup_list_node(entity_list_node_name);
    ally_list_node = setup_list_node(ally_list_node_name);
    enemy_list_node = setup_list_node(enemy_list_node_name);
}

Level * Level::clone() {
    UtilityFunctions::print("Go to sleep!");
    return nullptr; // TODO
}

Node * Level::setup_list_node(String list_node_name) {
    Node * list_node;
    if (this->has_node(list_node_name)) {
        list_node = this->get_node_or_null(list_node_name);
    } else {
        list_node = memnew(Node());
        this->add_child(list_node);
        list_node->set_name(list_node_name);
        list_node->set_owner(this);
    }
    
    return list_node;
}

bool Level::add_node(Node * new_child_node, Node * parent_node) {
    if (!new_child_node) {
        return false;
    }

    if (!parent_node) {
        parent_node = this;
    }

    if (this->has_node(player_list_node_name)) {
        player_list_node = this->get_node_or_null(player_list_node_name);
    } else {
        player_list_node = memnew(Node());
        player_list_node->set_name(player_list_node_name);
        this->add_child(player_list_node);
        player_list_node->set_owner(this);
    }
}