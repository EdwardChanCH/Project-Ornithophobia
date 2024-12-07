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
    ADD_SIGNAL(MethodInfo("show_results"));
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
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelController)
_GDEXPORT_GET(timeScaleFactor)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelController)
_GDEXPORT_SET(timeScaleFactor)
_GDEXPORT_SET_SUFFIX


/**
 * @brief Same as _ready() in GDScript.
 */
void LevelController::_ready() {
    set_process(false);
    connect("results_slow_time", Callable(get_parent()->find_child("SlowMotionMeter"), "_on_results_showing"));

    if (!Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(ProcessMode::PROCESS_MODE_ALWAYS);

        gameplayUI = memnew(CanvasLayer());

        levelUIScene = ResourceLoader::get_singleton()->load("res://screen/level_ui.tscn");
        levelUIInstance = Node::cast_to<Control>(levelUIScene->instantiate());
        gameplayUI->add_child(levelUIInstance);

        pauseScene = ResourceLoader::get_singleton()->load("res://screen/menu/pause_menu.tscn");
        pauseInstance = Node::cast_to<Control>(pauseScene->instantiate());
        pauseInstance->set_visible(false);
        gameplayUI->add_child(pauseInstance);

        resultsScreenScene = ResourceLoader::get_singleton()->load("res://screen/results_screen.tscn");
        resultsScreenInstance = Node::cast_to<Control>(resultsScreenScene->instantiate());
        resultsScreenInstance->set_visible(false);
        connect("show_results", Callable(resultsScreenInstance, "_on_results_screen_show"));
        gameplayUI->add_child(resultsScreenInstance);

        get_parent()->call_deferred("add_child", gameplayUI);

        enemyList = get_parent()->find_child("EnemyList");

        if (enemyList != nullptr) {
            numEnemies = enemyList->get_child_count();
            totalEnemies = numEnemies;
            cast_to<Label>(levelUIInstance->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));
        }
    }
}

/**
 * @brief Same as _input() in GDScript.
 */
void LevelController::_input(const Ref<InputEvent> &event) {
    if (event->is_action_pressed("escape") && UtilityFunctions::str(pauseInstance) != "<Freed Object>") {
        get_tree()->set_pause(!get_tree()->is_paused());
        if (get_tree()->is_paused()) {
            pauseInstance->set_visible(true);
        } else {
            pauseInstance->set_visible(false);
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
        emit_signal("show_results");
        UtilityFunctions::print(UtilityFunctions::str(Time::get_singleton()->get_ticks_msec() - slowLength));
        // instantiate results screen here
    }
}


void LevelController::_update_enemy_count() {
    if (numEnemies > 0) {
        numEnemies--;
        cast_to<Label>(levelUIInstance->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));

        if (numEnemies == 0) {
            slowLength = Time::get_singleton()->get_ticks_msec();
            set_process(true);
            emit_signal("results_slow_time");
            pauseInstance->queue_free();
        }
    }
}


void LevelController::_results_slow_time() {
    set_process(true);
}