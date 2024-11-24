/**
 * @file slow_motion_meter.cpp
 * @author
 * @brief
 * @version
 * @date
 * 
 */

#include "slow_motion_meter.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void SlowMotionMeter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_cooldown_timer_timeout"), &SlowMotionMeter::_on_cooldown_timer_timeout);
}

/**
 * @brief Construct a new SlowMotionMeter::SlowMotionMeter object.
 * 
 */
SlowMotionMeter::SlowMotionMeter() {
}

/**
 * @brief Destroy the SlowMotionMeter::SlowMotionMeter object.
 * 
 */
SlowMotionMeter::~SlowMotionMeter() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(SlowMotionMeter)
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "max_slow_time"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "max_slow_time_value"))
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "time_slow_factor"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "regen_rate"))
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "soft_max_value"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(SlowMotionMeter)
_GDEXPORT_GET(max_slow_time)
_GDEXPORT_GET(max_slow_time_value)
_GDEXPORT_GET(time_slow_factor)
_GDEXPORT_GET(regen_rate)
_GDEXPORT_GET(soft_max_value)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(SlowMotionMeter)
_GDEXPORT_SET(max_slow_time)
_GDEXPORT_SET(max_slow_time_value)
_GDEXPORT_SET(time_slow_factor)
_GDEXPORT_SET(regen_rate)
_GDEXPORT_SET(soft_max_value)
_GDEXPORT_SET_SUFFIX


void SlowMotionMeter::_ready() {
    max_time_slow_factor = max_slow_time / 100;
    animation_player = cast_to<AnimationPlayer>(find_child("AnimationPlayer"));
    cooldown = cast_to<Timer>(find_child("CooldownTimer"));
    cooldown->set_wait_time(1.0);
    set_value(soft_max_value);
    get_self_modulate().set_a8(0);
}


/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void SlowMotionMeter::_process(double delta) {
    value = get_value();
    // If the meter is full, allow it to fade out. Otherwise, stop the fade out animation
    if (get_value() == soft_max_value) {
        can_fade_out = true;
        can_fade_in = false;
    } else {
        can_fade_out = false;
        can_fade_in = true;

        if (animation_player->is_playing() && animation_player->get_current_animation() == "fade_out") {
            animation_player->stop();
        }
    }

    // Play fade out animation if meter is full
    if (value == soft_max_value && get_self_modulate().get_a8() == 255 && can_fade_out) {
        animation_player->play("fade_out");
    }

    // Prevent the slowing of time if the meter is empty
    can_slow_time = value > 0 ? true : false;

    update_meter();

    // Activate slow motion if it is able to be activated
	if (Input::get_singleton()->is_action_pressed("action_button") && can_drain && can_slow_time) {
        float new_time_scale = Engine::get_singleton()->get_time_scale() - delta * time_slow_factor;
		Engine::get_singleton()->set_time_scale(Math::clamp(new_time_scale, max_slow_time_value, 1.f));
    } else {
        float new_time_scale = Engine::get_singleton()->get_time_scale() + delta * time_slow_factor;
		Engine::get_singleton()->set_time_scale(Math::clamp(new_time_scale, max_slow_time_value, 1.f));
    }

    // If the meter can regenerate, start regenerating
    if (can_regenerate && (value < get_max())) {
        value += regen_rate * delta;
        if (value >= soft_max_value) {
            value = soft_max_value;
            can_regenerate = false;
        }
        set_value(value);
    }
    // Debug::get_singleton()->add_debug_property("soft_max_value", soft_max_value);
    // Debug::get_singleton()->add_debug_property("a", get_self_modulate().get_a8());
    // Debug::get_singleton()->add_debug_property("can_fade_in", can_fade_in);
    // Debug::get_singleton()->add_debug_property("can_fade_out", can_fade_out);
    // Debug::get_singleton()->add_debug_property("value", value);
	// Debug::get_singleton()->add_debug_property("can_drain", can_drain);
    // Debug::get_singleton()->add_debug_property("can_slow_time", can_slow_time);
}


void SlowMotionMeter::update_meter() {
    long time_pressed = 0;
	if (Input::get_singleton()->is_action_just_pressed("action_button")) {
        // Stop all currently playing animations
		if (animation_player->is_playing()) {
            animation_player->stop();
			animation_player->clear_queue();
        }
		
		// Set the max slow motion time to be dependent on how full the slow motion meter is
		max_slow_time = value * max_time_slow_factor;
		
		start_press_time = Time::get_singleton()->get_ticks_msec();
		can_regenerate = false;
		can_drain = true;
    }
	
	// If the action button is being held down, slow time
	if (Input::get_singleton()->is_action_pressed("action_button") && Engine::get_singleton()->get_time_scale() != 1 && can_drain) {
        // Update the amount of time the action button has been held down for
		time_pressed = Time::get_singleton()->get_ticks_msec() - start_press_time;
		// Stop regen cooldown timer if action button is pressed again
		cooldown->stop();
		// Play fade in animation if the slow motion bar isnt visible
		if (value < soft_max_value && get_self_modulate().get_a8() == 0 && can_fade_in)
			animation_player->play("fade_in", -1, 1 / Engine::get_singleton()->get_time_scale());
		
		// If the action button is held down and there is meter remaining, lower the meter
		if (time_pressed <= max_slow_time) {
            float new_value = ((max_slow_time) - time_pressed) / max_time_slow_factor;
			Math::clamp(new_value, 0.f, float(soft_max_value));
			value = new_value;
        } else {
            value = get_min();
			time_pressed = 0;
			can_drain = false;
			cooldown->stop();
			cooldown->start(cooldown->get_wait_time());
        }
        set_value(value);
    }
	
	// If the large blast button is released or cancelled, start the regen cooldown timer
	if ((Input::get_singleton()->is_action_just_released("action_button")) && cooldown->is_stopped() && !can_regenerate && value < soft_max_value)
		cooldown->start(cooldown->get_wait_time());
    
	// Debug::get_singleton()->add_debug_property("time_pressed", time_pressed);
}


void SlowMotionMeter::_on_cooldown_timer_timeout() {
    can_regenerate = true;
}