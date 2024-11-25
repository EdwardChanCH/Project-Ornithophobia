/**
 * @file animation_controller.cpp
 * @author Sameer Bugti
 * @brief
 * @version 1
 * @date November 21st, 2024.
 * 
 */

#include "animation_controller.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void AnimationController::_bind_methods() {
}

/**
 * @brief Construct a new ___Class Name Here___:: ___Class Name Here___ object.
 * 
 */
AnimationController::AnimationController() {
}

/**
 * @brief Destroy the ___Class Name Here___:: ___Class Name Here___ object.
 * 
 */
AnimationController::~AnimationController() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(AnimationController)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(AnimationController)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(AnimationController)
_GDEXPORT_SET_SUFFIX


void AnimationController::_ready() {
    parent = Node::cast_to<PlayerController>(get_parent());
    arms = Node::cast_to<AnimatedSprite2D>(find_child("Arms"));
}

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void AnimationController::_process(double delta) {
    Input::get_singleton()->get_axis("move_left", "move_right");
    Vector2 mouse = get_viewport()->get_mouse_position();
    arms->look_at(mouse);
    Vector2 offset = Vector2(4.0, 0.0);
    arms->set_offset(offset);

    if(get_global_position().x > mouse.x) {   //if the player is not facing the mouse
        set_flip_h(true);
        arms->set_flip_v(true);
    } else {
        set_flip_h(false);
        arms->set_flip_v(false);
    }

    if(parent->is_on_floor()) { //if the player is touching the floor
        Vector2 velocity = parent->get_velocity();

        if(velocity.length() == 0) {  //if the player is not moving
            play("idle");
            arms->play("idle");
        } else {
            if((velocity.x > 0 && !is_flipped_h()) || (velocity.x < 0 && is_flipped_h())) {  //if moving right and facing mouse, or if moving left and not facing mouse
                play("run");
                arms->play("run");
            } else if ((velocity.x < 0 && !is_flipped_h()) || (velocity.x > 0 && is_flipped_h())) { //if moving left and facing mouse, or if moving right and not facing mouse
                play_backwards("run");
                arms->play_backwards("run");
            }
        }
    } else {
        play("idleAir");
        arms->play("idleAir");
    }
}