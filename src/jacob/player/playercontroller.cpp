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
_GDEXPORT_GET(initialBlastStrength)
_GDEXPORT_GET(maxBlastStrength)
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
_GDEXPORT_SET(initialBlastStrength)
_GDEXPORT_SET(maxBlastStrength)
_GDEXPORT_SET_SUFFIX

void PlayerController::_ready() {
    set_process(true);
    debugNode = get_node<Label>("../UI/debug/speed");
    lastBlastTime = Time::get_singleton()->get_ticks_msec();
}

void PlayerController::_process(double _delta) {
    Vector2 vec2_zero = Vector2(0, 0);
    float delta = (float) _delta;
    Input *input = Input::get_singleton();
    float axis = input->get_axis("move_left", "move_right");

    isAirborne = !is_on_floor();

    bool wantToMove = axis != 0;

    // If an input is being pressed, update speed according
    if (wantToMove) {
        inputDirection = Vector2(axis, 0);

        // Smoothly accelerate and decelerate when attempting to move
        if ((get_velocity().x >= 0 && inputDirection.x == 1) || (get_velocity().x <= 0 && inputDirection.x == -1)) {
            // speed = Math::clamp(speed + groundAccel, 0.f, (float) maxGroundSpeed);
            if (speed < maxGroundSpeed)
                speed += groundAccel;
            else if (speed > maxGroundSpeed) {
                speed -= groundAccel * airFriction;
            }
        } else if ((get_velocity().x < 0 && inputDirection.x == 1) || (get_velocity().x > 0 && inputDirection.x == -1)) {
            // speed = Math::max(speed - groundDecel, 0.f);
            speed -= groundDecel;
        }
            
    } else {
        // speed = Math::max(speed - groundDecel, 0.f);
        speed -= groundDecel;
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
    Vector2 hVelocity = Vector2(get_velocity().x, 0);
    Vector2 vVelocity = Vector2(0, get_velocity().y);

    // If on ground and above max speed, apply ground friction
    if (!isAirborne && speed > maxGroundSpeed) {
        speed *= groundFriction;
    }

    // Set horizontal speed to 0 if touching wall
    if (is_on_wall()) {
        speed = 0;
    }

    // If the current velocity and input direction have different signs, decelerate and turn around
    if (movementDirection.x == 1 && get_velocity().x < 0 || movementDirection.x == -1 && get_velocity().x > 0)
        hVelocity = (movementDirection * -1) * speed;
    // Update velocity normally
    else { 
        hVelocity = movementDirection * speed;
    }

    // Start timer when blast button just pressed
    if (input->is_action_just_pressed("blast")) {
        blastTime = Time::get_singleton()->get_ticks_msec();
        blastStrength = initialBlastStrength;
        lastBlastTime = Time::get_singleton()->get_ticks_msec() - lastBlastTime;
    }

    // When blast button released, blast player in direction opposite to the mouse cursor
    if (input->is_action_just_released("blast")) {
        if (lastBlastTime >= 100) {
            blastTime = (Time::get_singleton()->get_ticks_msec() - blastTime);

            // Different levels of strength depending on how long the blast button was held for
            if (blastTime < 167)
                blastStrength = initialBlastStrength;
            else if (blastTime < 333)
                blastStrength = blastStrength * 1.67;
            else if (blastTime < 500)
                blastStrength = blastStrength * 2.33;
            else
                blastStrength = blastStrength * 3;

            Vector2 blastDirection = get_viewport()->get_mouse_position() - get_position();
            blastDirection *= -1;
            blastDirection.normalize();
            vVelocity = blastDirection * blastStrength;
            speed = vVelocity.x > 0 ? vVelocity.x : vVelocity.x * -1;
            
            isAirborne = true;
            blastTime = 0;
            lastBlastTime = Time::get_singleton()->get_ticks_msec();
        }
    }
    
    // Apply gravity
    if (isAirborne) {
        vVelocity.y += gravity;
        if (vVelocity.y > maxFallSpeed)
            vVelocity.y = maxFallSpeed;
    } else {
        fallSpeed = 0;
        vVelocity = vec2_zero;
    }

    // Reset player position if they fall offscreen
    Vector2 screenSize = get_viewport_rect().get_size();
    if (get_position().x < -64)
        set_position(Vector2(screenSize.x + 64, get_position().y));
    if (get_position().x > screenSize.x + 64)
        set_position(Vector2(-64, get_position().y));
    if (get_position().y < -64)
        set_position(Vector2(get_position().x, screenSize.y + 64));
    if (get_position().y > screenSize.y + 64)
        set_position(Vector2(get_position().x, -64));

    // Update player velocity
    set_velocity(hVelocity + vVelocity);
    
    // Debug log
    String debugText = "";
    debugText += "Speed: " + String::num(speed) + "\n";
    debugText += "VelocityX: " + String::num(get_velocity().x) + "\n";
    debugText += "VelocityY: " + String::num(vVelocity.y) + "\n";
    debugText += "input direction: " + String::num(inputDirection.x) + "\n";
    String isAirborneStr = isAirborne ? "true" : "false";
    debugText += "isAirborne: " + isAirborneStr + "\n";
    debugText += "Blast Strength: " + String::num(blastStrength) + "\n";
    debugNode->set_text(debugText);

    move_and_slide();
}
