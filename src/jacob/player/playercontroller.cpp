// This example class receives a signal from a GDExampleNode and output some text to the terminal. 

#include "playercontroller.h"

using namespace godot;

void PlayerController::_bind_methods() {
}

PlayerController::PlayerController() {
    speed = 0;
    isAirborne = false;
    blastStrength = 0;
}

PlayerController::~PlayerController() {
    // delete debugNode;
}

// Export instance variables to the Godot Editor. 
_GDEXPORT_ADD_PREFIX(PlayerController)
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxGroundSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "groundAccel"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "groundDecel"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "groundFriction"))

_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "gravity"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxAirSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "fallSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "maxFallSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "airFriction"))

_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "blastStrength"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "initialBlastStrength"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxBlastStrength"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxSmallBlastSpeed"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_GET_PREFIX(PlayerController)
_GDEXPORT_GET(maxGroundSpeed)
_GDEXPORT_GET(groundAccel)
_GDEXPORT_GET(groundDecel)
_GDEXPORT_GET(groundFriction)

_GDEXPORT_GET(gravity)
_GDEXPORT_GET(maxAirSpeed)
_GDEXPORT_GET(fallSpeed)
_GDEXPORT_GET(maxFallSpeed)
_GDEXPORT_GET(airFriction)

_GDEXPORT_GET(blastStrength)
_GDEXPORT_GET(smallBlastStrength)
_GDEXPORT_GET(largeBlastStrength)
_GDEXPORT_GET(maxSmallBlastSpeed)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_SET_PREFIX(PlayerController)
_GDEXPORT_SET(maxGroundSpeed)
_GDEXPORT_SET(groundAccel)
_GDEXPORT_SET(groundDecel)
_GDEXPORT_SET(groundFriction)

_GDEXPORT_SET(gravity)
_GDEXPORT_SET(maxAirSpeed)
_GDEXPORT_SET(fallSpeed)
_GDEXPORT_SET(maxFallSpeed)
_GDEXPORT_SET(airFriction)

_GDEXPORT_SET(blastStrength)
_GDEXPORT_SET(smallBlastStrength)
_GDEXPORT_SET(largeBlastStrength)
_GDEXPORT_SET(maxSmallBlastSpeed)
_GDEXPORT_SET_SUFFIX

void PlayerController::_ready() {
    set_process(true);
    debugNode = get_node<Label>("../UI/debug/speed");
    lastBlastTime = Time::get_singleton()->get_ticks_msec();
    wasOnWall = false;
}

void PlayerController::_process(double _delta) {
    Vector2 vec2_zero = Vector2(0, 0);
    float delta = (float) _delta;
    Input *input = Input::get_singleton();
    float axis = input->get_axis("move_left", "move_right");
    float airDecel = 0;

    isAirborne = !is_on_floor();

    bool wantToMove = axis != 0;

    // If an input is being pressed, update speed according
    if (wantToMove) {
        inputDirection = Vector2(axis, 0);

        // Smoothly accelerate and decelerate when attempting to move
        if ((get_velocity().x >= 0 && inputDirection.x == 1) || (get_velocity().x <= 0 && inputDirection.x == -1)) {
            if (isAirborne) {
                if (speed > maxGroundSpeed)
                    speed -= groundAccel * airFriction;
                else {
                    speed += groundAccel;
                    if (speed > maxGroundSpeed)
                        speed = maxGroundSpeed;
                }
            } else
                speed -= speed > maxGroundSpeed ? groundAccel * groundFriction : groundAccel * -1;
        } else if ((get_velocity().x < 0 && inputDirection.x == 1) || (get_velocity().x > 0 && inputDirection.x == -1)) {
            speed -= groundDecel;
            speed -= isAirborne ? groundDecel / airFriction : groundDecel / groundFriction;
        }
            
    } else {
        // If moving faster than the max air speed, increase strength of deceleration
        if (isAirborne && speed > maxAirSpeed) {
            airDecel = (speed / maxAirSpeed) * airFriction;
            if (airDecel < 1) airDecel = 1;
            speed -= groundAccel * airDecel;
            // UtilityFunctions::print("Speed: " + String::num(speed) + "\nAir Decel: " + String::num(airDecel));
        } else if (!isAirborne) {
            speed -= groundDecel;
        }

        if (movementDirection.x == 0)
            movementDirection.x = 1;
    }

    // Clamp speed
    if (speed < 0)
        speed = 0;

    // Normalize input direction vector
    float _inputDirLen = inputDirection.length();
    if (_inputDirLen > 0.03f) {
        if (_inputDirLen > 1)
            inputDirection = inputDirection.normalized();
        movementDirection = inputDirection;
    }

    // Velocities
    Vector2 velocity = Vector2(get_velocity().x, get_velocity().y);

    // If on ground and above max speed, apply ground friction
    if (!isAirborne && speed > maxGroundSpeed) {
        speed *= groundFriction;
    }

    // Set horizontal speed to 0 if touching wall
    if (is_on_wall()) {
        speed = Math::clamp(speed, -groundAccel, groundAccel);
    }

    // If the current velocity and input direction have different signs, decelerate and turn around
    if (movementDirection.x == 1 && get_velocity().x < 0 || movementDirection.x == -1 && get_velocity().x > 0)
        velocity.x = (movementDirection.x * -1) * speed;
    // Update velocity normally
    else { 
        velocity.x = movementDirection.x * speed;
    }

    // Start timer when blast button just pressed
    if (input->is_action_just_pressed("small_blast") || input->is_action_just_pressed("large_blast")) {
        // blastTime = Time::get_singleton()->get_ticks_msec();
        lastBlastTime = Time::get_singleton()->get_ticks_msec() - lastBlastTime;
    }

    // When blast button released, blast player in direction opposite to the mouse cursor
    if (input->is_action_just_released("small_blast") || input->is_action_just_released("large_blast")) {
        if (lastBlastTime >= 75) {
            if (input->is_action_just_released("small_blast"))
                blastStrength = smallBlastStrength;
            else
                blastStrength = largeBlastStrength;
            Vector2 blastDirection = get_viewport()->get_mouse_position() - get_position();
            blastDirection *= -1;
            blastDirection.normalize();
            // UtilityFunctions::print("blastDirX: " + String::num(blastDirection.x) + "\nblastDirY: " + String::num(blastDirection.y));

            // Update horizontal speed correctly
            if (blastDirection.x < 0 && velocity.x >= 0) {              // blasting from right when moving right
                velocity.x += blastDirection.x * blastStrength;
            } else if (blastDirection.x > 0 && velocity.x > 0) {       // blasting from left while moving right
                if (velocity.x > maxSmallBlastSpeed)
                    velocity.x += (blastDirection.x * blastStrength) / (velocity.x / maxSmallBlastSpeed);
                else
                    velocity.x += blastDirection.x * blastStrength;
            } else if (blastDirection.x < 0 && velocity.x < 0) {       // blasting from right while moving left
                if (velocity.x < maxSmallBlastSpeed * -1)
                    velocity.x += (blastDirection.x * blastStrength) / (velocity.x / (maxSmallBlastSpeed * -1));
                else
                    velocity.x += blastDirection.x * blastStrength;
            } else if (blastDirection.x > 0 && velocity.x <= 0) {       // blasting from left while moving left
                velocity.x += blastDirection.x * blastStrength;
            }
            
            // Update vertical speed correctly
            if (blastDirection.y < 0 && velocity.y >= 0) {              // blasting from below when moving down
                velocity.y = 0;
                velocity.y = blastDirection.y * blastStrength;
            } else if (blastDirection.y > 0 && velocity.y > 0) {       // blasting from above while moving down
                if (velocity.y > maxSmallBlastSpeed)
                    velocity.y += (blastDirection.y * blastStrength) / (velocity.y / maxSmallBlastSpeed);
                else
                    velocity.y += blastDirection.y * blastStrength;
            } else if (blastDirection.y < 0 && velocity.y < 0) {       // blasting from below while moving up
                if (velocity.y < maxSmallBlastSpeed * -1)
                    velocity.y += (blastDirection.y * blastStrength) / (velocity.y / (maxSmallBlastSpeed * -1));
                else
                    velocity.y += blastDirection.y * blastStrength;
            } else if (blastDirection.y > 0 && velocity.y <= 0) {       // blasting from above while moving up
                velocity.y = 0;
                velocity.y = blastDirection.y * blastStrength;
            }
            
            // UtilityFunctions::print("hVel: " + String::num(velocity.x) + "\nvVel: " + String::num(velocity.y));
            velocity.x = Math::clamp(velocity.x, -750.f, 750.f);
            velocity.y = Math::clamp(velocity.y, -750.f, 750.f);
            speed = velocity.x > 0 ? velocity.x : velocity.x * -1;
            speed = Math::clamp(speed, -750.f, 750.f);
            
            isAirborne = true;
            blastTime = 0;
            lastBlastTime = Time::get_singleton()->get_ticks_msec();
        }
    }

    // When blast button released, blast player in direction opposite to the mouse cursor
    if (input->is_action_just_released("large_blast")) {
        if (lastBlastTime >= 200) {
            blastStrength = 750;
            Vector2 blastDirection = get_viewport()->get_mouse_position() - get_position();
            blastDirection *= -1;
            blastDirection.normalize();
            velocity = blastDirection * blastStrength;
            speed = velocity.x > 0 ? velocity.x : velocity.x * -1;
            
            isAirborne = true;
            blastTime = 0;
            lastBlastTime = Time::get_singleton()->get_ticks_msec();
        }
    }
    
    // Apply gravity
    if (isAirborne) {
        // If moving faster than the max air speed, increase strength of gravity
        if (velocity.y < (maxAirSpeed * -1)) {
            float vAirDecel = (velocity.y / (maxAirSpeed * -1)) * airFriction;
            if (vAirDecel < 1) vAirDecel = 1;
            velocity.y -= gravity * vAirDecel * delta * -1;
        } else {
            velocity.y -= gravity * delta * -1;
        }
        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;
    } else {
        fallSpeed = 0;
        velocity.y = 0;
    }

    int screenWrapBuffer = 16;
    // Reset player position if they fall offscreen
    Vector2 screenSize = get_viewport_rect().get_size();
    if (get_position().x < -screenWrapBuffer)
        set_position(Vector2(screenSize.x + screenWrapBuffer, get_position().y));
    if (get_position().x > screenSize.x + screenWrapBuffer)
        set_position(Vector2(-screenWrapBuffer, get_position().y));
    if (get_position().y < -screenWrapBuffer)
        set_position(Vector2(get_position().x, screenSize.y + screenWrapBuffer));
    if (get_position().y > screenSize.y + screenWrapBuffer)
        set_position(Vector2(get_position().x, -screenWrapBuffer));

    // Update player velocity
    set_velocity(velocity);
    
    // Debug log
    String debugText = "";
    debugText += "Speed: " + String::num(speed) + "\n";
    debugText += "VelocityX: " + String::num(get_velocity().x) + "\n";
    debugText += "VelocityY: " + String::num(velocity.y) + "\n";
    debugText += "input direction: " + String::num(inputDirection.x) + "\n";
    String isAirborneStr = isAirborne ? "true" : "false";
    debugText += "isAirborne: " + isAirborneStr + "\n";
    debugText += "Blast Strength: " + String::num(blastStrength) + "\n";
    debugText += "Air Decel: " + String::num(airDecel) + "\n";
    
    // Set debug label text
    debugNode->set_text(debugText);
    
    move_and_slide();
}
