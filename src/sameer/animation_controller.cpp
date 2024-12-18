/**
 * @file animation_controller.cpp
 * @author Sameer Bugti
 * @brief
 * @version 1
 * @date November 25th, 2024.
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
    input = Input::get_singleton();
    tempVFX = ResourceLoader::get_singleton()->load("res://object/VFX.tscn");
}

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void AnimationController::_process(double delta) {  //constantly updating process called every tick
    Vector2 mouse = get_viewport()->get_mouse_position();   //grabs current mouse position as a 2 variable Vector
    arms->look_at(mouse);   //makes the node containing the arm sprite look at the mouse's position
    Vector2 offset = Vector2(4.0, 0.0); //creates vector for offsetting the arms
    arms->set_offset(offset);   //offsets the arms to make their rotation look more natural

    if(get_global_position().x > mouse.x) {   //if the player is not facing the mouse
        set_flip_h(true);   //flip the character to face the mouse
        arms->set_flip_v(true); //vertically flip the arm so it is still standing upright
    } else {    //if the player is facing the mouse
        set_flip_h(false);  //dont flip the character
        arms->set_flip_v(false);    //dont flip the arm
    }

    if(parent->is_on_floor()) { //if the player is touching the floor
        Vector2 velocity = parent->get_velocity();  //grab the current velocity through the parent node

        if(velocity.length() == 0) {  //if the player is not moving
            play("idle");   //play idle animation for both body and arms
            arms->play("idle");
        } else {
            if((velocity.x > 0 && !is_flipped_h()) || (velocity.x < 0 && is_flipped_h())) {  //if moving right and facing mouse, or if moving left and not facing mouse
                play("run");    //play running animation forward for both body and arms
                arms->play("run");
            } else if ((velocity.x < 0 && !is_flipped_h()) || (velocity.x > 0 && is_flipped_h())) { //if moving left and facing mouse, or if moving right and not facing mouse
                play_backwards("run");  //play backwards running animation for both body and arms
                arms->play_backwards("run");
            }
        }
    } else {    //if the player is in the air
        play("idleAir");    //play idling air animation for both body and arms
        arms->play("idleAir");
    }

     if (input->is_action_just_pressed("small_blast") || input->is_action_just_pressed("large_blast")) {
        AnimatedSprite2D *vfx = Node::cast_to<AnimatedSprite2D>(tempVFX->instantiate());
        get_tree()->get_root()->add_child(vfx);
        vfx->set_position(parent->get_position());
        vfx->look_at(mouse);
        if (input->is_action_just_pressed("small_blast")) {
            vfx->play("left");
        } else {
            vfx->play("right");
        }
     }
}