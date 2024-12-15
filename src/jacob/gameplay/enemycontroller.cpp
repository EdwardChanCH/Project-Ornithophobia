/**
 * @file enemycontroller.cpp
 * @author Jacob Couture
 * @brief This class handles states and animations for an enemy.
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
    ADD_SIGNAL(MethodInfo("enemy_died"));
    ADD_SIGNAL(MethodInfo("bounce_player", PropertyInfo(Variant::VECTOR2, "enemy_pos")));
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
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "minKillSpeed"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(EnemyController)
_GDEXPORT_GET(minKillSpeed)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(EnemyController)
_GDEXPORT_SET(minKillSpeed)
_GDEXPORT_SET_SUFFIX


/**
 * @brief Same as Godot's _ready() function
 */
void EnemyController::_ready() {
    if (!is_connected("enemy_died", Callable(find_child("SFXPlayer"), "_on_enemy_died")))
        connect("enemy_died", Callable(find_child("SFXPlayer"), "_on_enemy_died"));
    if (!is_connected("bounce_player", Callable(find_child("SFXPlayer"), "_on_bounce_player")))
        connect("bounce_player", Callable(find_child("SFXPlayer"), "_on_bounce_player"));
    deathAnim = Node::cast_to<AnimatedSprite2D>(find_child("DeathAnimation"));
    deathAnim->stop();
}


/**
 * @brief Receiver for detecting when the player controller enters this enemy's collider
 * @param body The Node2D that entered the collision of this Enemy node
 */
void EnemyController::_on_player_controller_entered(Node2D *body) {
    if (body->get_class() == "PlayerController") {
        if (Node::cast_to<PlayerController>(body)->get_velocity().length() >= minKillSpeed) {
            if (body->get_name() == UtilityFunctions::str("PlayerController") && !deathAnim->is_playing()) {
                deathAnim->set_visible(true);
                deathAnim->play("death");
                emit_signal("enemy_died");
                find_child("Area2D")->queue_free();
            }
        } else {
            emit_signal("bounce_player", get_position());
        }
    }
}


/**
 * @brief Same as Godot's _process() function
 * @param delta The time passed since the last frame
 */
void EnemyController::_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        // Hide enemy sprite at the peak of the explosion
        if (deathAnim->is_playing() && deathAnim->get_frame() == 8) {
            Node::cast_to<Sprite2D>(find_child("EnemySprite"))->set_visible(false);
        }

        // Stop animation when it reaches the last frame. Delete this enemy after
        if (deathAnim->is_playing() && deathAnim->get_frame() == deathAnim->get_sprite_frames()->get_frame_count("death") - 1) {
            deathAnim->stop();
            queue_free();
        }
    }
}