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

}

// Export instance variables to the Godot Editor. 
_GDEXPORT_ADD_PREFIX(PlayerController)
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "maxGroundSpeed"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "groundAccel"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "groundDecel"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_GET_PREFIX(PlayerController)
_GDEXPORT_GET(maxGroundSpeed)
_GDEXPORT_GET(groundAccel)
_GDEXPORT_GET(groundDecel)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor. 
_GDEXPORT_SET_PREFIX(PlayerController)
_GDEXPORT_SET(maxGroundSpeed)
_GDEXPORT_SET(groundAccel)
_GDEXPORT_SET(groundDecel)
_GDEXPORT_SET_SUFFIX

void PlayerController::_ready() {
    set_process(true);
    debugNode = get_node<Label>("../UI/debug/speed");
}

void PlayerController::_process(double _delta) {
    float delta = (float) _delta;
    Input *input = Input::get_singleton();
    float axis = input->get_axis("move_left", "move_right");

    bool wantToMove = axis != 0;

    if (wantToMove) {
        inputDirection = Vector2(axis, 0);
        if ((get_velocity().x >= 0 && inputDirection.x == 1) || (get_velocity().x <= 0 && inputDirection.x == -1))
            speed = Math::clamp(speed + groundAccel, 0.f, (float) maxGroundSpeed);
        else if ((get_velocity().x < 0 && inputDirection.x == 1) || (get_velocity().x > 0 && inputDirection.x == -1))
            speed = Math::max(speed - groundDecel, 0.f);
    } else {
        speed = Math::max(speed - groundDecel, 0.f);
    }

    float _inputDirLen = inputDirection.length();
    if (_inputDirLen > 0.03f) {
        if (_inputDirLen > 1)
            inputDirection = inputDirection.normalized();
        movementDirection = inputDirection;
    }

    Vector2 velocity = Vector2(0, 0);

    if (movementDirection.x == 1 && get_velocity().x < 0 || movementDirection.x == -1 && get_velocity().x > 0)
        velocity.x += (movementDirection.x * -1) * speed;
    else
        velocity.x += movementDirection.x * speed;

    set_velocity(velocity);

    // Debug log
    debugNode->set_text(
        "Speed: " + String::num(speed) + "\n"
        + "axis: " + String::num(axis) + "\n"
        + "Velocity: " + String::num(get_velocity().x) + "\n"
        + "input direction: " + String::num(inputDirection.x) + "\n"
        );

    move_and_slide();
}
