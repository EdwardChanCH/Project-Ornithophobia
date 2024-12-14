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
    ClassDB::bind_method(D_METHOD("set_level"), &LevelController::set_level);
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
_GDEXPORT_ADD(PropertyInfo(Variant::STRING, "level_icon"))
_GDEXPORT_ADD(PropertyInfo(Variant::PACKED_STRING_ARRAY, "rank_times"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelController)
_GDEXPORT_GET(timeScaleFactor)
_GDEXPORT_GET(numEnemies)

_GDEXPORT_GET(level_name)
_GDEXPORT_GET(level_author)
_GDEXPORT_GET(level_icon)
_GDEXPORT_GET(rank_times)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelController)
_GDEXPORT_SET(timeScaleFactor)
_GDEXPORT_SET(numEnemies)

_GDEXPORT_SET(level_name)
_GDEXPORT_SET(level_author)
_GDEXPORT_SET(level_icon)
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
        
        if (levelNode != nullptr) {
            levelNode->set_physics(true);
            // add_child(levelNode);
        }


        /*-------------------- Set up Camera --------------------*/


        camera = memnew(Camera2D());
        levelNode->call_deferred("add_child", camera);
        camera->set_offset(Vector2(960, 540));
        camera->set_script(ResourceLoader::get_singleton()->load("res://script/camera_2d.gd"));


        /*-------------------- Set up UI --------------------*/


        gameplayUI = memnew(CanvasLayer());

        // Instantiate level ui
        levelUINode = (Control*) SceneManager::get_instance()->import_scene_tscn("res://screen/level_ui.tscn");
        gameplayUI->add_child(levelUINode);

        // Instantiate pause menu
        pauseScreenNode = (Control*) SceneManager::get_instance()->import_scene_tscn("res://screen/menu/pause_menu.tscn");
        pauseScreenNode->set_visible(false);
        gameplayUI->add_child(pauseScreenNode);

        // Instantiate results screen and connect the "show_results" signal to it
        resultsScreenNode = (Control*) SceneManager::get_instance()->import_scene_tscn("res://screen/results_screen.tscn");
        resultsScreenNode->set_visible(false);
        connect("show_results", Callable(resultsScreenNode, "_on_results_screen_show"));
        gameplayUI->add_child(resultsScreenNode);

        // Add gameplayUI to the scene tree once setup is complete
        add_child(gameplayUI);


        /*-------------------- Set up enemies/player --------------------*/


        enemyList = levelNode->find_child("EnemyList");
        playerList = levelNode->find_child("PlayerList");
        PlayerController* player = cast_to<PlayerController>(playerList->get_children().front());

        if (enemyList != nullptr) {
            numEnemies = enemyList->get_child_count();

            TypedArray<Node> enemies = enemyList->get_children();
            for (int i = 0; i < numEnemies; i++) {
                EnemyController* enemy = cast_to<EnemyController>(enemies[i]);
                enemy->connect("enemy_died", Callable(this, "_update_enemy_count"));
                enemy->connect("enemy_died", Callable(camera, "apply_shake"));
                enemy->connect("bounce_player", Callable(player, "_collide_with_enemy"));
            }

            totalEnemies = numEnemies;
            cast_to<Label>(levelUINode->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));
        }

        if (playerList != nullptr) {
            player->set_process_mode(ProcessMode::PROCESS_MODE_INHERIT);
            connect("results_slow_time", Callable(player->find_child("SlowMotionMeter"), "_on_results_showing"));
        }


        /*-------------------- Set children process mode --------------------*/


        levelNode->set_process_mode(ProcessMode::PROCESS_MODE_PAUSABLE);
        gameplayUI->set_process_mode(ProcessMode::PROCESS_MODE_PAUSABLE);
    }
}

/**
 * @brief Same as _input() in GDScript.
 */
void LevelController::_input(const Ref<InputEvent> &event) {
    // Pause the game
    if (event->is_action_pressed("escape") && UtilityFunctions::str(pauseScreenNode) != "<Freed Object>") {
        get_tree()->set_pause(!get_tree()->is_paused());
        if (get_tree()->is_paused()) {
            pauseScreenNode->set_visible(true);
        } else {
            pauseScreenNode->set_visible(false);
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
        if (event->is_action_pressed("debug_force_results") && !pauseScreenNode->is_visible()) {
            get_tree()->set_pause(false);
            resultsScreenNode->set_visible(false);
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
        levelUINode->queue_free();
        pauseScreenNode->queue_free();
        resultsScreenNode->queue_free();
        gameplayUI->queue_free();
        camera->queue_free();
        levelNode->queue_free();
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
        level_metadata.get_or_add("level_icon", level_icon);
        level_metadata.get_or_add("level_time", ((Label*) levelUINode->find_child("LevelTimer"))->get_text());
        level_metadata.get_or_add("best_time", best_time);
        level_metadata.get_or_add("rank_times", rank_times);
        level_metadata.get_or_add("rank_icon", rank_icon);
        level_metadata.get_or_add("rank_flavour_text", rank_flavour_text);
        emit_signal("show_results", level_metadata);
    }
}


void LevelController::_update_enemy_count() {
    if (numEnemies > 0) {
        numEnemies--;
        cast_to<Label>(levelUINode->find_child("EnemiesRemaining"))->set_text("Enemies Remaining: " + UtilityFunctions::str(numEnemies));

        if (numEnemies == 0) {
            slowLength = Time::get_singleton()->get_ticks_msec();
            // set_process(true);
            emit_signal("results_slow_time");
            pauseScreenNode->queue_free();
        }
    }
}


void LevelController::_results_slow_time() {
    set_process(true);
}


void LevelController::calculate_best_time() {
    String cur_time = cast_to<LevelTimer>(levelUINode->find_child("LevelTimer"))->get_text();
    if (best_time == "--:--.--" || read_formatted_time(best_time) < read_formatted_time(cur_time)) {
        best_time = cur_time;
    }
}


int LevelController::read_formatted_time(String time) {
    PackedStringArray units = time.replace(":", " ").replace(".", " ").split(" ");
    return (units[0].to_int() * 60) + units[1].to_int() + float(units[2].to_int()) * 0.01;
}


void LevelController::calculate_rank() {
    int cur_time = read_formatted_time(cast_to<LevelTimer>(levelUINode->find_child("LevelTimer"))->get_text());

    gold_rank_text = Array({"SWEET MOVES!", "PERFECT!", "AMAZING!"});
    silver_rank_text = Array({"RADICAL!", "ARTISTIC!", "COOL!"});
    bronze_rank_text = Array({"GOOD!", "GREAT!", "NICE!"});

    if (cur_time < read_formatted_time(rank_times[0])) {
        rank_icon = "res://asset/sprite/menu/gold_rank_icon.png";
        rank_flavour_text = gold_rank_text.pick_random();
    } else if (cur_time < read_formatted_time(rank_times[1])) {
        rank_icon = "res://asset/sprite/menu/silver_rank_icon.png";
        rank_flavour_text = silver_rank_text.pick_random();
    } else if (cur_time < read_formatted_time(rank_times[2])) {
        rank_icon = "res://asset/sprite/menu/bronze_rank_icon.png";
        rank_flavour_text = bronze_rank_text.pick_random();
    } else {
        rank_icon = "res://asset/sprite/menu/no_rank_icon.png";
        rank_flavour_text = no_rank_text;
    }
}


void LevelController::set_level(String level_path) {
    levelNode = (Level*) SceneManager::get_instance()->import_scene_tscn(level_path);
    level_metadata = levelNode->get_level_info();
    level_name = level_metadata.get_or_add("level_name", "null");
    level_author = level_metadata.get_or_add("level_author", "null");
    level_icon = level_metadata.get_or_add("level_icon", "res://asset/sprite/default_texture.png");
    best_time = level_metadata.get_or_add("best_time", "--:--.--");
    rank_times = level_metadata.get_or_add("rank_times", PackedStringArray(Array({"00:00.00", "00:00.00", "00:00.00"})));
    add_child(levelNode);
}