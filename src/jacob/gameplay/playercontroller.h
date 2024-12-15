/**
 * @file playercontroller.h
 * @author Jacob Couture
 * @brief Header file for the PlayerController class
 */

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code.
#include "globals.h"
// #include "inputhandler.h"
#include "debug.h"

namespace godot {

    class PlayerController : public CharacterBody2D {
        
        GDCLASS(PlayerController, CharacterBody2D)
        _GDEXPORT

    private:
        // General movement variables
        float speed;
        Vector2 inputDirection;
        Vector2 movementDirection;
        Vector2 bounceVelocity;
        bool isAirborne;
        bool wasOnWall;
        bool wasOnFloor;
        bool onFloor;

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
        int trueMaxSpeed;

        // Blast variables
        float blastStrength;
        int smallBlastStrength = 250;
        int largeBlastStrength = 750;
        int maxSmallBlastSpeed = 375;
        int maxLargeBlastSpeed = 500;
        long blastTime;
        long lastBlastTime;

        // Misc.
        Input *input;
        int enemyBounceStrength;


    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        virtual void _ready() override;

        PlayerController();
        ~PlayerController();

        void _physics_process(double delta) override;
        float update_blast_velocity(float blastDir, float vel, int maxBlastSpeed, String direction="");
        bool was_on_floor();
        void _collide_with_enemy(Vector2 enemy_pos);

    };

}

#endif