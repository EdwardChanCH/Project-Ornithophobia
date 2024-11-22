/**
 * @file enemycontroller.cpp
 * @author
 * @brief
 * @version
 * @date
 * 
 */

#include "enemycontroller.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void EnemyController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_player_controller_entered", "body"), &EnemyController::_on_player_controller_entered);
}

/**
 * @brief Construct a new EnemyController::EnemyController object.
 * 
 */
EnemyController::EnemyController() {
}

/**
 * @brief Destroy the EnemyController::EnemyController object.
 * 
 */
EnemyController::~EnemyController() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(EnemyController)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(EnemyController)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(EnemyController)
_GDEXPORT_SET_SUFFIX


void EnemyController::_ready() {
    deathAnim = Node::cast_to<AnimatedSprite2D>(find_child("DeathAnimation"));
    deathAnim->stop();
}


void EnemyController::_on_player_controller_entered(Node2D *body) {
    if (body->get_name() == UtilityFunctions::str("PlayerController")) {
        deathAnim->set_visible(true);
        deathAnim->play("death");
    }
}


void EnemyController::_process(double delta) {
    if (deathAnim->is_playing() && deathAnim->get_frame() == 8) {
        Node::cast_to<Sprite2D>(find_child("EnemySprite"))->set_visible(false);
    }

    if (deathAnim->is_playing() && deathAnim->get_frame() == deathAnim->get_sprite_frames()->get_frame_count("death") - 1) {
        deathAnim->stop();
        queue_free();
    }
}