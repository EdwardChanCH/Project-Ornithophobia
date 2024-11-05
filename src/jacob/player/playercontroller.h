#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/label.hpp>
// #include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code. 
// #include "inputhandler.h"

namespace godot {

    class PlayerController : public CharacterBody2D {
        
        GDCLASS(PlayerController, CharacterBody2D)
        // _GDEXPORT

    private:
        // Debug variables
        Label *debugNode;

        // General movement variables
        float speed;
        Vector2 inputDirection;
        Vector2 movementDirection;
        bool isAirborne;

        // Ground variables
        const int maxGroundSpeed = 60;
        const float groundAccel = 4;
        const float groundDecel = 2;
        const float groundFriction = 0.5;

        // Air variables
        const float gravity = 5;
        const int maxAirSpeed = 20;
        const float airFriction = 0.2;

        // Blast variables
        float blastStrength;
        const int initialBlastStrength = 5;
        const int maxBlastStrength = 20;


    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        virtual void _ready() override;

        PlayerController();
        ~PlayerController();

        void _process(double delta) override;

        // getters/setters

    };

}

#endif