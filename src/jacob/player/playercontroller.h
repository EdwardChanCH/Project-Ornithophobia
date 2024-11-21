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
        float timeSlowValue;
        bool canSlowTime;
        Input *input;


    protected:
        // this function must be declared.
        static void _bind_methods();

    public:
        virtual void _ready() override;
        virtual void _exit_tree() override;

        PlayerController();
        ~PlayerController();

        void _process(double delta) override;
        void set_game_speed(float gameSpeed);
        float update_blast_velocity(float blastDir, float vel, int maxBlastSpeed, String direction="");
        bool can_slow_time();
        void set_can_slow_time(bool value);
        bool was_on_floor();

    };

}

#endif