/**
 * @file level_controller.cpp
 * @author Jacob Couture
 * @brief This class handles the initialization and control of a level.
 */

#include "level_controller.h"

// Everything in GDExtension is defined within the namespace "godot". 
using namespace godot;

/**
 * @brief Binding function for exposing methods/properties to Godot.
 */
void LevelController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_update_enemy_count"), &LevelController::_update_enemy_count);
    ClassDB::bind_method(D_METHOD("_results_slow_time"), &LevelController::_results_slow_time);
    ADD_SIGNAL(MethodInfo("results_slow_time"));
    ADD_SIGNAL(MethodInfo("show_results", PropertyInfo(Variant::DICTIONARY, "level_metadata")));
}

/**
 * @brief Constructor for LevelController
 */
LevelController::LevelController() {
}

/**
 * @brief Destructor for LevelController
 */
LevelController::~LevelController() {
}


// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(LevelController)
_GDEXPORT_ADD(PropertyInfo(Variant::FLOAT, "timeScaleFactor"))

_GDEXPORT_ADD(PropertyInfo(Variant::STRING, "level_name"))
_GDEXPORT_ADD(PropertyInfo(Variant::STRING, "level_author"))
_GDEXPORT_ADD(PropertyInfo(Variant::STRING, "level_icon_path"))
_GDEXPORT_ADD(PropertyInfo(Variant::PACKED_STRING_ARRAY, "rank_times"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelController)
_GDEXPORT_GET(timeScaleFactor)
_GDEXPORT_GET(numEnemies)

_GDEXPORT_GET(level_name)
_GDEXPORT_GET(level_author)
_GDEXPORT_GET(level_icon_path)
_GDEXPORT_GET(rank_times)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelController)
_GDEXPORT_SET(timeScaleFactor)
_GDEXPORT_SET(numEnemies)

_GDEXPORT_SET(level_name)
_GDEXPORT_SET(level_author)
_GDEXPORT_SET(level_icon_path)
_GDEXPORT_SET(rank_times)
_GDEXPORT_SET_SUFFIX


/**
 * @brief Same as _ready() in GDScript.
 */
void LevelController::_ready() {
    set_process(false);
    connect("results_slow_time", Callable(this, "_results_slow_time"));

    if (!Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(ProcessMode::PROCESS_MODE_ALWAYS);

        gameplayUI = memnew(CanvasLayer());

        // Instantiate level ui
        levelUIScene = ResourceLoader::get_singleton()->load("res://screen/level_ui.tscn");
        levelUIInstance = Node::cast_to<Control>(levelUIScene->instantiate());
        gameplayUI->add_child(levelUIInstance);

        // Instantiate pause menu
        pauseScene = ResourceLoader::get_singleton()->load("res://screen/menu/pause_menu.tscn");
        pauseInstance = Node::cast_to<Control>(pauseScene->instantiate());
        pauseInstance->set_visible(false);
        gameplayUI->add_child(pauseInstance);

        // Instantiate results screen and connect the "show_results" signal to it
        resultsScreenScene = ResourceLoader::get_singleton()->load("res://screen/results_screen.tscn");
        resultsScreenInstance = Node::cast_to<Control>(resultsScreenScene->instantiate());
        resultsScreenInstance->set_visible(false);
        connect("show_results", Callable(resultsScreenInstance, "_on_results_screen_show"));
        gameplayUI->add_child(resultsScreenInstance);

        get_parent()->call_deferred("add_child", gameplayUI);

        enemyList = get_parent()->find_child("EnemyList");
        playerList = get_parent()->find_child("PlayerList");

        if (enemyList != nullptr) {
            numEnemies = enemyList->get_child_count();
            totalEnemies = numEnemies;
            cast_to<Label>(levelUIInstance->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));
        }

        if (playerList != nullptr) {
            if (playerList->get_child_count() > 0) {
                PlayerController* player = cast_to<PlayerController>(playerList->get_children().front());
                player->set_process_mode(ProcessMode::PROCESS_MODE_INHERIT);
                connect("results_slow_time", Callable(player->find_child("SlowMotionMeter"), "_on_results_showing"));
            }
        }
    }
}

/**
 * @brief Same as _input() in GDScript.
 */
void LevelController::_input(const Ref<InputEvent> &event) {
    // Pause the game
    if (event->is_action_pressed("escape") && UtilityFunctions::str(pauseInstance) != "<Freed Object>") {
        get_tree()->set_pause(!get_tree()->is_paused());
        if (get_tree()->is_paused()) {
            pauseInstance->set_visible(true);
        } else {
            pauseInstance->set_visible(false);
        }
    }

    // Debug inputs
    if (Debug::get_singleton()->is_debug_mode_active()) {
        // Reload scene if debug mode active
        if (event->is_action_pressed("debug_reload_scene")) {
            get_tree()->set_pause(false);
            Engine::get_singleton()->set_time_scale(1.0);
            get_tree()->reload_current_scene();
        }

        // Force results screen to appear
        if (event->is_action_pressed("debug_force_results") && !pauseInstance->is_visible()) {
            get_tree()->set_pause(false);
            resultsScreenInstance->set_visible(false);
            slowLength = Time::get_singleton()->get_ticks_msec();
            emit_signal("results_slow_time");
        }
    }
}

/**
 * @brief Same as _exit_tree() in GDScript.
 */
void LevelController::_exit_tree() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        levelUIInstance->queue_free();
        pauseInstance->queue_free();
        resultsScreenInstance->queue_free();
        gameplayUI->queue_free();
    }
}


void LevelController::_process(double delta) {
    float newTimeScale = Engine::get_singleton()->get_time_scale() - timeScaleFactor;
    Engine::get_singleton()->set_time_scale(newTimeScale);

    if (Engine::get_singleton()->get_time_scale() <= 0) {
        get_tree()->set_pause(true);
        Engine::get_singleton()->set_time_scale(1.0);
        set_process(false);

        calculate_best_time();
        if (!rank_times.is_empty())
            calculate_rank();

        Dictionary level_metadata = Dictionary();
        level_metadata.get_or_add("level_name", level_name);
        level_metadata.get_or_add("level_author", level_author);
        level_metadata.get_or_add("level_icon_path", level_icon_path);
        level_metadata.get_or_add("best_time", best_time);
        level_metadata.get_or_add("rank_times", rank_times);
        level_metadata.get_or_add("rank_icon_path", rank_icon_path);
        emit_signal("show_results", level_metadata);
    }
}


void LevelController::_update_enemy_count() {
    if (numEnemies > 0) {
        numEnemies--;
        cast_to<Label>(levelUIInstance->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));

        if (numEnemies == 0) {
            slowLength = Time::get_singleton()->get_ticks_msec();
            // set_process(true);
            emit_signal("results_slow_time");
            pauseInstance->queue_free();
        }
    }
}


void LevelController::_results_slow_time() {
    set_process(true);
}


void LevelController::calculate_best_time() {
    String cur_time = cast_to<LevelTimer>(levelUIInstance->find_child("LevelTimer"))->get_text();
    if (best_time == "--:--.--" || read_formatted_time(best_time) < read_formatted_time(cur_time)) {
        best_time = cur_time;
    }
}


int LevelController::read_formatted_time(String time) {
    PackedStringArray units = time.replace(":", " ").replace(".", " ").split(" ");
    return (units[0].to_int() * 60) + units[1].to_int() + float(units[2].to_int()) * 0.01;
}


void LevelController::calculate_rank() {
    int cur_time = read_formatted_time(cast_to<LevelTimer>(levelUIInstance->find_child("LevelTimer"))->get_text());
    
    if (cur_time < read_formatted_time(rank_times[0])) {
        rank_icon_path = "res://asset/sprite/menu/gold_rank_icon.png";
    } else if (cur_time < read_formatted_time(rank_times[1])) {
        rank_icon_path = "res://asset/sprite/menu/silver_rank_icon.png";
    } else if (cur_time < read_formatted_time(rank_times[2])) {
        rank_icon_path = "res://asset/sprite/menu/bronze_rank_icon.png";
    } else {
        rank_icon_path = "res://asset/sprite/menu/no_rank_icon.png";
    }
}