// This example class receives a signal from a GDExampleNode and output some text to the terminal.

#include "playercontroller.h"

using namespace godot;

void PlayerController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_game_speed", "gameSpeed"), &PlayerController::set_game_speed);
    ClassDB::bind_method(D_METHOD("update_blast_velocity", "blastDir", "vel", "maxBlastSpeed", "direction"), &PlayerController::update_blast_velocity);
    ClassDB::bind_method(D_METHOD("can_slow_time"), &PlayerController::can_slow_time);
    ClassDB::bind_method(D_METHOD("set_can_slow_time", "value"), &PlayerController::set_can_slow_time);
    ClassDB::bind_method(D_METHOD("was_on_floor"), &PlayerController::was_on_floor);
}

PlayerController::PlayerController() {
    speed = 0;
    isAirborne = false;
    blastStrength = 0;
    input = Input::get_singleton();
}

PlayerController::~PlayerController() {
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
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "trueMaxSpeed"))

_GDEXPORT_ADD(PropertyInfo(Variant::INT, "smallBlastStrength"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "largeBlastStrength"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxSmallBlastSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxLargeBlastSpeed"))

_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "timeSlowValue", PROPERTY_HINT_RANGE, "0,1,0.01"))
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
_GDEXPORT_GET(trueMaxSpeed)

_GDEXPORT_GET(smallBlastStrength)
_GDEXPORT_GET(largeBlastStrength)
_GDEXPORT_GET(maxSmallBlastSpeed)
_GDEXPORT_GET(maxLargeBlastSpeed)

_GDEXPORT_GET(timeSlowValue)
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
_GDEXPORT_SET(trueMaxSpeed)

_GDEXPORT_SET(smallBlastStrength)
_GDEXPORT_SET(largeBlastStrength)
_GDEXPORT_SET(maxSmallBlastSpeed)
_GDEXPORT_SET(maxLargeBlastSpeed)

_GDEXPORT_SET(timeSlowValue)
_GDEXPORT_SET_SUFFIX

void PlayerController::_ready() {
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    else {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);
    }

    lastBlastTime = Time::get_singleton()->get_ticks_msec();
    wasOnWall = false;
    wasOnFloor = false;
    canSlowTime = true;
    movementDirection.x = 1;
}

void PlayerController::_exit_tree() {
}

void PlayerController::_process(double _delta) {
    float delta = (float) _delta;
    float axis = input->get_axis("move_left", "move_right");
    float airDecel = 0;
    bool wantToMove = axis != 0 ? true : false;
    isAirborne = !is_on_floor();

    // If an input is being pressed, update speed according
    if (wantToMove) {
        inputDirection = Vector2(axis, 0);

        // Smoothly accelerate and decelerate when attempting to move
        if ((get_velocity().x >= 0 && inputDirection.x == 1) || (get_velocity().x <= 0 && inputDirection.x == -1)) {
            if (isAirborne) {
                if (speed < maxGroundSpeed) {
                    speed += groundAccel;
                    if (speed > maxGroundSpeed)
                        speed = maxGroundSpeed;
                }
            } else
                speed -= speed > maxGroundSpeed ? groundAccel * groundFriction : groundAccel * -1;
        }
        // If inputting the opposite direction the player is moving in, decelerate smoothly
        else if ((get_velocity().x < 0 && inputDirection.x == 1) || (get_velocity().x > 0 && inputDirection.x == -1)) {
            speed -= isAirborne ? groundDecel / airFriction : groundAccel + groundDecel / groundFriction;
        }
            
    }

    // If moving faster than the max air speed, increase strength of deceleration
    if (isAirborne && speed > maxAirSpeed) {
        airDecel = (speed / maxAirSpeed) * airFriction;
        if (airDecel < 1) airDecel = 1;
        speed -= wantToMove ? groundDecel * airDecel * (airFriction / 1.25) : groundDecel * airDecel;       // Slow down deceleration if player is holding down a movement direction
    } else if (!isAirborne && !wantToMove) {
        speed -= groundDecel;
    }

    // Clamp speed
    if (speed < 0) speed = 0;

    // Normalize input direction vector
    float _inputDirLen = inputDirection.length();
    if (_inputDirLen > 0.03f) {
        if (_inputDirLen > 1)
            inputDirection = inputDirection.normalized();
        movementDirection = inputDirection;
    }


    /*-------------------------------------------------------------------------------------------------------------*/


    // Velocities
    Vector2 velocity = Vector2(get_velocity().x, get_velocity().y);

    // If on ground and above max speed, apply ground friction
    if (!isAirborne && speed > maxGroundSpeed) {
        speed *= groundFriction;
    }

    // Set horizontal speed to 0 if touching wall
    if (is_on_wall()) {
        if (wantToMove)
            speed = groundAccel;
        else
            speed = 0;
    }

    // Check if player just left the floor
    if (onFloor != is_on_floor()) {
        if (onFloor)
            wasOnFloor = true;
    } else {
        wasOnFloor = false;
    }
    onFloor = is_on_floor();

    // If the current velocity and input direction have different signs, decelerate and turn around
    if (movementDirection.x == 1 && get_velocity().x < 0 || movementDirection.x == -1 && get_velocity().x > 0)
        velocity.x = (movementDirection.x * -1) * speed;
    // Update velocity normally
    else
        velocity.x = movementDirection.x * speed;
    
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
    } else {        // If on ground, do not apply gravity
        fallSpeed = 0;
        velocity.y = 0;
    }


    /*-------------------------------------------------------------------------------------------------------------*/


    // Start timer when blast button just pressed
    if (input->is_action_just_pressed("small_blast") || input->is_action_just_pressed("large_blast")) {        
        blastTime = Time::get_singleton()->get_ticks_msec();
        lastBlastTime = Time::get_singleton()->get_ticks_msec() - lastBlastTime;
    }

    // Slow motion code
    if (input->is_action_pressed("action_button") && canSlowTime) {
        set_game_speed(timeSlowValue);
    }

    if (input->is_action_just_released("action_button") || !canSlowTime) {
        set_game_speed(1);
    }

    // When blast button released, blast player in direction opposite to the mouse cursor
    if ((input->is_action_just_released("small_blast") && lastBlastTime >= 75) || (input->is_action_just_released("large_blast") && lastBlastTime >= 150)) {
        // Get the direction the player will move from the blast based on the position of the mouse
        Vector2 blastDirection = get_viewport()->get_mouse_position() - get_global_position();
        blastDirection *= -1;
        blastDirection.normalize();

        if (input->is_action_just_released("small_blast")) {
            blastStrength = smallBlastStrength;
            // Update velocities based on direction of the blast and the current direction the player is moving
            velocity.x = update_blast_velocity(blastDirection.x, velocity.x, maxSmallBlastSpeed);
            velocity.y = update_blast_velocity(blastDirection.y, velocity.y, maxSmallBlastSpeed, "vertical");
        } else {
            blastStrength = largeBlastStrength;
            velocity.x = update_blast_velocity(blastDirection.x, velocity.x, maxLargeBlastSpeed);
            velocity.y = update_blast_velocity(blastDirection.y, velocity.y, maxLargeBlastSpeed, "vertical");
        }

        // Clamp velocity and speed
        velocity.x = Math::clamp(velocity.x, float(-trueMaxSpeed), float(trueMaxSpeed));
        velocity.y = Math::clamp(velocity.y, float(-trueMaxSpeed), float(trueMaxSpeed));
        speed = velocity.x > 0 ? velocity.x : velocity.x * -1;
        speed = Math::clamp(speed, float(-trueMaxSpeed), float(trueMaxSpeed));
        
        isAirborne = true;
        blastTime = 0;
        lastBlastTime = Time::get_singleton()->get_ticks_msec();
    }


    /*-------------------------------------------------------------------------------------------------------------*/


    // Update player velocity
    set_velocity(velocity);
    
    // Debug log
    Debug::get_singleton()->add_debug_property("FPS", UtilityFunctions::snappedf((1.0 / delta), 0.01));
    Debug::get_singleton()->add_debug_property("speed", UtilityFunctions::snappedf(speed, 0.01));
    Debug::get_singleton()->add_debug_property("velocityX", UtilityFunctions::snappedf(get_velocity().x, 0.01));
    Debug::get_singleton()->add_debug_property("velocityY", UtilityFunctions::snappedf(velocity.y, 0.01));
    Debug::get_singleton()->add_debug_property("isAirborne", isAirborne);
    Debug::get_singleton()->add_debug_property("airDecel", UtilityFunctions::snappedf(airDecel, 0.01));

    // Move the player
    move_and_slide();
    
}

void PlayerController::set_game_speed(float gameSpeed) {
    Engine::get_singleton()->set_time_scale(gameSpeed);
}

// Calculates the correct velocity based on the direction of a blast and the current velocity of the player
float PlayerController::update_blast_velocity(float blastDir, float vel, int maxBlastSpeed, String direction) {
    if ((blastDir < 0 && vel >= 0) ) {          // blasting from right when moving right
        if (direction == "vertical")
            vel = 0;
        vel += blastDir * blastStrength;
    } else if (blastDir > 0 && vel > 0) {       // blasting from left while moving right
        if (vel > maxBlastSpeed)
            vel += (blastDir * blastStrength) / (vel / maxBlastSpeed);
        else
            vel += blastDir * blastStrength;
    } else if (blastDir < 0 && vel < 0) {       // blasting from right while moving left
        if (vel < maxBlastSpeed * -1)
            vel += (blastDir * blastStrength) / (vel / (maxBlastSpeed * -1));
        else
            vel += blastDir * blastStrength;
    } else if (blastDir > 0 && vel <= 0) {      // blasting from left while moving left
        vel += blastDir * blastStrength;
    }

    return vel;
}

bool PlayerController::can_slow_time() {
    return canSlowTime;
}

void PlayerController::set_can_slow_time(bool value) {
    canSlowTime = value;
}

bool PlayerController::was_on_floor() {
    return wasOnFloor;
}