#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/time.hpp>
// #include <../timecontroller.h>
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code.
#include "globals.h"
// #include "inputhandler.h"

namespace godot {

    class PlayerController : public CharacterBody2D {
        
        GDCLASS(PlayerController, CharacterBody2D)
        _GDEXPORT

    private:
        // Debug variables
        Label *debugNode;

        // General movement variables
        float speed;
        Vector2 inputDirection;
        Vector2 movementDirection;
        bool isAirborne;
        bool wasOnWall;

        // Ground variables
        int maxGroundSpeed;
        float groundAccel;
        float groundDecel;
        float groundFriction;

        // Air variables
        float gravity;
        int maxAirSpeed;
        float fallSpeed;
        int maxFallSpeed;
        float airFriction;

        // Blast variables
        float blastStrength = 250;
        int smallBlastStrength = 250;
        int largeBlastStrength = 750;
        int maxSmallBlastSpeed = 375;
        long blastTime;
        long lastBlastTime;

        // Misc.
        float timeSlowValue;
        bool canSlowTime;
        Input *input = Input::get_singleton();


    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        virtual void _ready() override;

        PlayerController();
        ~PlayerController();

        void _process(double delta) override;
        void set_game_speed(float gameSpeed);
        void updateSmallBlastVelocity(float *blastDir, float *vel, String direction="");

        // getters/setters

    };

}

#endif