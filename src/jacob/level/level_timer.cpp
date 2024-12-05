/**
 * @file level_timer.cpp
 * @author Jacob Couture
 * @brief Acts as a pauseable level timer to display on-screen
 */

#include "level_timer.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void LevelTimer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("format_time", "time"), &LevelTimer::format_time);
}

/**
 * @brief Construct a new ___Class Name Here___:: ___Class Name Here___ object.
 * 
 */
LevelTimer::LevelTimer() {
}

/**
 * @brief Destroy the ___Class Name Here___:: ___Class Name Here___ object.
 * 
 */
LevelTimer::~LevelTimer() {
}

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(LevelTimer)
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelTimer)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelTimer)
_GDEXPORT_SET_SUFFIX

void LevelTimer::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(ProcessMode::PROCESS_MODE_INHERIT);
    } else {
        set_process_mode(ProcessMode::PROCESS_MODE_DISABLED);
    }
    start_time = Time::get_singleton()->get_ticks_msec();
    pause_time = 0;
}

/**
 * @brief Same as _process() in GDScript.
 * 
 * @param delta Delta time
 */
void LevelTimer::_process(double delta) {
    start_time = float(start_time) + delta * 1000;
    set_text(format_time(start_time));
}

String LevelTimer::format_time(uint64_t time) {
    int minutes = time / 60000;
    int seconds = (time - (minutes * 60000)) / 1000;
    int milliseconds = (time - (minutes * 60000) - (seconds * 1000)) / 10;
    String strMinutes = String::num_int64(minutes);
    String strSeconds = String::num_int64(seconds);
    String strMillis = String::num_int64(milliseconds);

    if (minutes < 10) {
        strMinutes = "0" + strMinutes;
    }

    if (seconds < 10) {
        strSeconds = "0" + strSeconds;
    }

    if (milliseconds < 10) {
        strMillis = "0" + strMillis;
    }

    return strMinutes + ":" + strSeconds + "." + strMillis;
}