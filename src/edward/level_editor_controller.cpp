/**
 * @file level_editor_controller.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Mediator Pattern (MVC).
 * This class controlls all communication between Godot UI (view) and Level data (model).
 * @version 0.3.0
 * @date 2024-11-22
 * 
 */

#include "level_editor_controller.h"
#include "scene_manager.h"

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

// Export instance variables to the Godot Editor
_GDEXPORT_ADD_PREFIX(LevelEditorController)
_GDEXPORT_ADD(PropertyInfo(Variant::INT, "undo_limit"))
_GDEXPORT_ADD_SUFFIX

// Getter(s) for exported instance variables in Godot Editor
_GDEXPORT_GET_PREFIX(LevelEditorController)
_GDEXPORT_GET(undo_limit)
_GDEXPORT_GET_SUFFIX

// Setter(s) for exported instance variables in Godot Editor
_GDEXPORT_SET_PREFIX(LevelEditorController)
_GDEXPORT_SET(undo_limit)
_GDEXPORT_SET_SUFFIX

/**
 * @brief Binding function for exposing methods/ properties to Godot.
 * 
 */
void LevelEditorController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("do_action", "new_action", "reverse_action"), &LevelEditorController::do_action);
    ClassDB::bind_method(D_METHOD("undo_action"), &LevelEditorController::undo_action);
    ClassDB::bind_method(D_METHOD("redo_action"), &LevelEditorController::redo_action);
    ClassDB::bind_method(D_METHOD("clear_action"), &LevelEditorController::clear_action);
    ClassDB::bind_method(D_METHOD("can_undo"), &LevelEditorController::can_undo);
    ClassDB::bind_method(D_METHOD("can_redo"), &LevelEditorController::can_redo);

    ClassDB::bind_method(D_METHOD("load_level", "filepath"), &LevelEditorController::load_level);
    ClassDB::bind_method(D_METHOD("save_level", "filepath"), &LevelEditorController::save_level);
    ClassDB::bind_method(D_METHOD("unload_level"), &LevelEditorController::unload_level);
    ClassDB::bind_method(D_METHOD("reload_level"), &LevelEditorController::reload_level);

    ClassDB::bind_method(D_METHOD("_test_print", "s", "n"), &LevelEditorController::_test_print);
    ClassDB::bind_method(D_METHOD("_debug"), &LevelEditorController::_debug);

    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_open_previous_screen"), &LevelEditorController::_on_level_editor_ui_open_previous_screen);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_playtest_button_toggled", "active"), &LevelEditorController::_on_level_editor_ui_playtest_button_toggled);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_debug_level_editor_controller"), &LevelEditorController::_on_level_editor_ui_debug_level_editor_controller);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_test_action_button_pressed", "n"), &LevelEditorController::_on_level_editor_ui_test_action_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_undo_button_pressed"), &LevelEditorController::_on_level_editor_ui_undo_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_redo_button_pressed"), &LevelEditorController::_on_level_editor_ui_redo_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_load_level_path_selected", "filepath"), &LevelEditorController::_on_level_editor_ui_load_level_path_selected);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_save_level_path_selected", "filepath"), &LevelEditorController::_on_level_editor_ui_save_level_path_selected);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_quick_load_level_button_pressed"), &LevelEditorController::_on_level_editor_ui_quick_load_level_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_quick_save_level_button_pressed"), &LevelEditorController::_on_level_editor_ui_quick_save_level_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_unload_level_button_pressed"), &LevelEditorController::_on_level_editor_ui_unload_level_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_reload_level_button_pressed"), &LevelEditorController::_on_level_editor_ui_reload_level_button_pressed);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_screen_left_clicked", "mouse_pos"), &LevelEditorController::_on_level_editor_ui_screen_left_clicked);
    ClassDB::bind_method(D_METHOD("_on_level_editor_ui_screen_right_clicked", "mouse_pos"), &LevelEditorController::_on_level_editor_ui_screen_right_clicked);
}

/**
 * @brief Construct a new Level Editor Controller:: Level Editor Controller object.
 * 
 */
LevelEditorController::LevelEditorController() {
    level_node = nullptr;

    undo_stack = memnew(TypedArray<Callable>);
    redo_stack = memnew(TypedArray<Callable>);
    action_index = 0;
    undo_limit = default_undo_limit;
    level_filepath = "";
}

/**
 * @brief Destroy the Level Editor Controller:: Level Editor Controller object.
 * 
 */
LevelEditorController::~LevelEditorController() {
    if (ui_node) {
        ui_scene.unref();
        ui_node = nullptr;
    }

    memdelete(undo_stack);
    undo_stack = nullptr;
    memdelete(redo_stack);
    redo_stack = nullptr;
}

/**
 * @brief Called when this node is ready in Godot's scene tree.
 * 
 */
void LevelEditorController::_ready() {
    // Load Godot UI
    if (ResourceLoader::get_singleton()->exists(ui_filepath, "PackedScene")) {
        CanvasLayer *ui_canvas = memnew(CanvasLayer);
        ui_canvas->set_layer(100); // Draw on top of everything else
        add_child(ui_canvas);

        ui_scene = ResourceLoader::get_singleton()->load(ui_filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        ui_node = ui_scene.ptr()->instantiate();
        ui_canvas->add_child(ui_node);

        // Connect Godot UI signals
        ui_node->connect("open_previous_screen", Callable(this, "_on_level_editor_ui_open_previous_screen"));
        ui_node->connect("playtest_button_toggled", Callable(this, "_on_level_editor_ui_playtest_button_toggled"));
        ui_node->connect("debug_level_editor_controller", Callable(this, "_on_level_editor_ui_debug_level_editor_controller"));
        ui_node->connect("test_action_button_pressed", Callable(this, "_on_level_editor_ui_test_action_button_pressed"));
        ui_node->connect("undo_button_pressed", Callable(this, "_on_level_editor_ui_undo_button_pressed"));
        ui_node->connect("redo_button_pressed", Callable(this, "_on_level_editor_ui_redo_button_pressed"));
        ui_node->connect("load_level_path_selected", Callable(this, "_on_level_editor_ui_load_level_path_selected"));
        ui_node->connect("save_level_path_selected", Callable(this, "_on_level_editor_ui_save_level_path_selected"));
        ui_node->connect("quick_load_level_button_pressed", Callable(this, "_on_level_editor_ui_quick_load_level_button_pressed"));
        ui_node->connect("quick_save_level_button_pressed", Callable(this, "_on_level_editor_ui_quick_save_level_button_pressed"));
        ui_node->connect("unload_level_button_pressed", Callable(this, "_on_level_editor_ui_unload_level_button_pressed"));
        ui_node->connect("reload_level_button_pressed", Callable(this, "_on_level_editor_ui_reload_level_button_pressed"));
        ui_node->connect("screen_left_clicked", Callable(this, "_on_level_editor_ui_screen_left_clicked"));
        ui_node->connect("screen_right_clicked", Callable(this, "_on_level_editor_ui_screen_right_clicked"));
    }
}

// - - - Action Handling Functions - - -

/**
 * @brief Track a new action and execute it, and track its reverse action.
 * 
 * @param new_action New action
 * @param reverse_action Reserse action
 * @return Variant Output of calling new_action
 */
Variant LevelEditorController::do_action(Callable new_action, Callable reverse_action) {
    // Remove actions in old timeline
    while (action_index < redo_stack->size()) {
        undo_stack->pop_back();
        redo_stack->pop_back();
    }

    // Track new action
    undo_stack->push_back(reverse_action);
    redo_stack->push_back(new_action);
    ++action_index;

    if (action_index > undo_limit) {
        // Remove oldest action
        undo_stack->pop_front();
        redo_stack->pop_front();
        --action_index;
    }

    // Execute action
    return new_action.call();
}

/**
 * @brief Execute the last undo-able action.
 * 
 * @return Variant Output of the last undo-able action
 */
Variant LevelEditorController::undo_action() {
    if (!can_undo()) {
        // No more undo available
        UtilityFunctions::print("Warning: No undo available.");
        return Variant::NIL;
    }

    // Find undo action
    --action_index;
    Callable action = (*undo_stack)[action_index];

    // Execute action
    return action.call();
}

/**
 * @brief Execute the last redo-able action.
 * 
 * @return Variant Output of the last redo-able action
 */
Variant LevelEditorController::redo_action() {
    if (!can_redo()) {
        // No more redo available
        UtilityFunctions::print("Warning: No redo available.");
        return Variant::NIL;
    }

    // Find redo action
    Callable action = (*redo_stack)[action_index];
    ++action_index;

    // Execute action
    return action.call();
}

/**
 * @brief Clear all tracked undo/ redo actions.
 * 
 */
void LevelEditorController::clear_action() {
    undo_stack->clear();
    redo_stack->clear();
    action_index = 0;
}

/**
 * @brief Check if any undoable action is available.
 * 
 * @return true 
 * @return false 
 */
bool LevelEditorController::can_undo() {
    // Check if there is at least 1 undoable action
    return (action_index > 0);
}

/**
 * @brief Check if any redoable action is available.
 * 
 * @return true 
 * @return false 
 */
bool LevelEditorController::can_redo() {
    // Check if there is at least 1 redoable action
    return (action_index < redo_stack->size());
}

// - - - File Handling Functions - - -

void LevelEditorController::load_level(String filepath) {
    if (ResourceLoader::get_singleton()->exists(filepath, "PackedScene")) {
        unload_level();
        level_filepath = filepath;
        level_scene = ResourceLoader::get_singleton()->load(level_filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        level_node = level_scene.ptr()->instantiate();
        level_node->set_process_mode(PROCESS_MODE_DISABLED); // Disable physics simulation
        add_child(level_node);
    } else {
        UtilityFunctions::print("Warning: Cannot find level at '", filepath,"' !");
    }
}

void LevelEditorController::save_level(String filepath) {
    if ((level_node) && (filepath.length() > 0)) {
        // Recursively set children as 'owned' for packing
        TypedArray<Node> family = level_node->get_children(false);
        Node * child;
        while (!family.is_empty()) {
            child = (Node *)(Object *)family.pop_front();
            child->set_owner(level_node);
            family.append_array(child->get_children());
        }

        level_filepath = filepath;
        level_scene.reference_ptr(memnew(PackedScene));
        level_scene.ptr()->pack(level_node);
        ResourceSaver::get_singleton()->save(level_scene, level_filepath, 0);
    } else {
        UtilityFunctions::print("Warning: Nothing to save!");
    }

}

void LevelEditorController::quick_load_level() {
    load_level(level_filepath.get_basename().replace("_quicksave", "") + "_quicksave" + "." + level_filepath.get_extension());
}

void LevelEditorController::quick_save_level() {
    save_level(level_filepath.get_basename().replace("_quicksave", "") + "_quicksave" + "." + level_filepath.get_extension());
}

void LevelEditorController::unload_level() {
    if (level_node) {
		level_node->queue_free();
		level_node = nullptr;
    }
}

void LevelEditorController::reload_level() {
    if (ResourceLoader::get_singleton()->exists(level_filepath, "PackedScene")) {
        load_level(level_filepath);
    }
}

// - - - Tile Editing Functions - - -

Vector2i LevelEditorController::world_pos_to_tile_pos(Vector2 world_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2i result;

    result.x = floor(world_pos.x / tile_map_scale.x / (real_t)tile_size.x);
    result.y = floor(world_pos.y / tile_map_scale.y / (real_t)tile_size.y);
    return result;
}

Vector2 LevelEditorController::tile_pos_to_world_pos(Vector2i tile_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2 result;

    result.x = ((real_t)tile_pos.x * tile_map_scale.x * tile_size.x);
    result.y = ((real_t)tile_pos.y * tile_map_scale.y * tile_size.y);
    return result;
}
	
// - - - Test Functions - - -

/**
 * @brief Test function.
 * 
 * @param s String
 * @param n Integer
 */
void LevelEditorController::_test_print(String s, int n) {
    UtilityFunctions::print(s, n);
}

/**
 * @brief Debug function.
 * 
 */
void LevelEditorController::_debug() {
    UtilityFunctions::print("------------------------------");

    UtilityFunctions::print("Class: LevelEditorController");

    UtilityFunctions::print("Undo Stack:");
    for (int i = 0; i < undo_stack->size(); ++i) {
        UtilityFunctions::print((*undo_stack)[i]);
    }

    UtilityFunctions::print("Redo Stack:");
    for (int i = 0; i < redo_stack->size(); ++i) {
        UtilityFunctions::print((*redo_stack)[i]);
    }

    UtilityFunctions::print("Action Index:", action_index);

    UtilityFunctions::print("Undo Limit:", undo_limit);

    UtilityFunctions::print("------------------------------");
}

// - - - Godot UI Signal Receivers - - -

void LevelEditorController::_on_level_editor_ui_open_previous_screen() {
    SceneManager::get_instance()->load_previous_scene(this->get_tree());
}

void LevelEditorController::_on_level_editor_ui_playtest_button_toggled(bool active) {
    if (level_node) {
        if (active) {
            quick_save_level();
            level_node->set_process_mode(PROCESS_MODE_ALWAYS);
        } else {
            quick_load_level();
        }
    }
}

void LevelEditorController::_on_level_editor_ui_debug_level_editor_controller() {
    _debug();
}

void LevelEditorController::_on_level_editor_ui_test_action_button_pressed(int n) {
    Callable actionA = Callable(this, "_test_print").bind("ReDo:", n);
    Callable actionB = Callable(this, "_test_print").bind("Undo:", n);
    do_action(actionA, actionB);
}

void LevelEditorController::_on_level_editor_ui_undo_button_pressed() {
    undo_action();
}

void LevelEditorController::_on_level_editor_ui_redo_button_pressed() {
    redo_action();
}

void LevelEditorController::_on_level_editor_ui_load_level_path_selected(String filepath) {
    load_level(filepath);
}

void LevelEditorController::_on_level_editor_ui_save_level_path_selected(String filepath) {
    save_level(filepath);
}

void LevelEditorController::_on_level_editor_ui_quick_load_level_button_pressed() {
    quick_load_level();
}

void LevelEditorController::_on_level_editor_ui_quick_save_level_button_pressed() {
    quick_save_level();
}

void LevelEditorController::_on_level_editor_ui_unload_level_button_pressed() {
    unload_level();
}

void LevelEditorController::_on_level_editor_ui_reload_level_button_pressed() {
    reload_level();
}

void LevelEditorController::_on_level_editor_ui_screen_left_clicked(Vector2 mouse_pos) {
    if (level_node) {
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_child(1));
        Ref<TileSet> tile_set = tile_map_layer->get_tile_set();

        Vector2i tile_pos = world_pos_to_tile_pos(mouse_pos, tile_map_layer->get_scale(), tile_set.ptr()->get_tile_size());
        Vector2 world_pos = tile_pos_to_world_pos(tile_pos, tile_map_layer->get_scale(), tile_set.ptr()->get_tile_size());

        UtilityFunctions::print(mouse_pos);
        UtilityFunctions::print(tile_pos);
        UtilityFunctions::print(world_pos);
        UtilityFunctions::print("----------");

        tile_map_layer->set_cell(tile_pos, 0, Vector2(0, 0), 0);
    }
    // TODO
}

void LevelEditorController::_on_level_editor_ui_screen_right_clicked(Vector2 mouse_pos) {
    if (level_node) {
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_child(1));
        Ref<TileSet> tile_set = tile_map_layer->get_tile_set();


        Vector2i world_pos1 = world_pos_to_tile_pos(mouse_pos, tile_map_layer->get_scale(), tile_set.ptr()->get_tile_size());
        Vector2 world_pos2 = tile_pos_to_world_pos(world_pos1, tile_map_layer->get_scale(), tile_set.ptr()->get_tile_size());

        Ref<PackedScene> reimu_scene = ResourceLoader::get_singleton()->load("res://entity/yinyang_orb.tscn", "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        RigidBody2D * reimu_node = (RigidBody2D *)reimu_scene.ptr()->instantiate();
        reimu_node->set_position(world_pos2 + Vector2(8, 8));
        level_node->get_child(2)->add_child(reimu_node);
        reimu_node->set_owner(reimu_node->get_parent()->get_parent());
    }
    // TODO
}
