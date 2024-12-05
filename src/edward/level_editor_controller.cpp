/**
 * @file level_editor_controller.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Mediator Pattern (MVC).
 * This class controlls all communication between Godot UI (view) and Level data (model).
 * @version 0.3.1
 * @date 2024-11-25
 * 
 */

#define BASE_FILEPATH(filepath) \
filepath.replace("_quicksave", "")

#define QUICK_FILEPATH(filepath) \
filepath.get_basename().replace("_quicksave", "") + "_quicksave" + "." + filepath.get_extension()

#include "level_editor_controller.h"

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
    ADD_SIGNAL(MethodInfo("tile_type_changed", PropertyInfo(Variant::VECTOR2I, "atlas_coords")));
    ADD_SIGNAL(MethodInfo("tile_alt_changed", PropertyInfo(Variant::INT, "tile_alt")));

    ClassDB::bind_method(D_METHOD("track_action", "redo_action", "undo_action"), &LevelEditorController::track_action);
    ClassDB::bind_method(D_METHOD("undo_action"), &LevelEditorController::undo_action);
    ClassDB::bind_method(D_METHOD("redo_action"), &LevelEditorController::redo_action);
    ClassDB::bind_method(D_METHOD("clear_action"), &LevelEditorController::clear_action);
    ClassDB::bind_method(D_METHOD("can_undo"), &LevelEditorController::can_undo);
    ClassDB::bind_method(D_METHOD("can_redo"), &LevelEditorController::can_redo);

    ClassDB::bind_method(D_METHOD("set_level_filepath", "filepath"), &LevelEditorController::set_level_filepath);
    ClassDB::bind_method(D_METHOD("load_new_level", "filepath"), &LevelEditorController::load_new_level);
    ClassDB::bind_method(D_METHOD("load_level", "filepath"), &LevelEditorController::load_level);
    ClassDB::bind_method(D_METHOD("save_level", "filepath"), &LevelEditorController::save_level);
    ClassDB::bind_method(D_METHOD("quick_load_level"), &LevelEditorController::quick_load_level);
    ClassDB::bind_method(D_METHOD("quick_save_level"), &LevelEditorController::quick_save_level);
    ClassDB::bind_method(D_METHOD("unload_level"), &LevelEditorController::unload_level);
    ClassDB::bind_method(D_METHOD("reload_level"), &LevelEditorController::reload_level);
    ClassDB::bind_method(D_METHOD("is_level_loaded"), &LevelEditorController::is_level_loaded);

    ClassDB::bind_method(D_METHOD("toggle_physics"), &LevelEditorController::toggle_physics);
    ClassDB::bind_method(D_METHOD("world_to_tile_pos", "world_pos", "tile_map_scale", "tile_size"), &LevelEditorController::world_to_tile_pos);
    ClassDB::bind_method(D_METHOD("tile_to_world_pos", "tile_pos", "tile_map_scale", "tile_size"), &LevelEditorController::tile_to_world_pos);
    ClassDB::bind_method(D_METHOD("get_tile_alt"), &LevelEditorController::get_tile_alt);
    
    ClassDB::bind_method(D_METHOD("flip_tile", "mode"), &LevelEditorController::flip_tile);
    ClassDB::bind_method(D_METHOD("rotate_tile", "clockwise"), &LevelEditorController::rotate_tile);
    ClassDB::bind_method(D_METHOD("cycle_tile", "next"), &LevelEditorController::cycle_tile);

    ClassDB::bind_method(D_METHOD("add_tile", "mouse_pos"), &LevelEditorController::add_tile);
    ClassDB::bind_method(D_METHOD("remove_tile", "mouse_pos"), &LevelEditorController::remove_tile);
    ClassDB::bind_method(D_METHOD("replace_tile", "tile_pos", "source_id", "tile_id", "tile_alt"), &LevelEditorController::replace_tile);
    ClassDB::bind_method(D_METHOD("add_scene_object", "mouse_pos", "list_name", "scene_path"), &LevelEditorController::add_scene_object);
    ClassDB::bind_method(D_METHOD("add_player", "mouse_pos"), &LevelEditorController::add_player);
    ClassDB::bind_method(D_METHOD("add_enemy", "mouse_pos"), &LevelEditorController::add_enemy);
    ClassDB::bind_method(D_METHOD("add_entity", "mouse_pos"), &LevelEditorController::add_entity);

    ClassDB::bind_method(D_METHOD("_test_action", "n"), &LevelEditorController::_test_action);
    ClassDB::bind_method(D_METHOD("_debug"), &LevelEditorController::_debug);
}

/**
 * @brief Construct a new Level Editor Controller:: Level Editor Controller object.
 * 
 */
LevelEditorController::LevelEditorController() {
    level_node = nullptr;
    level_filepath = default_level_filepath;

    undo_stack = memnew(TypedArray<Callable>);
    redo_stack = memnew(TypedArray<Callable>);
    action_index = 0;
    quick_save_action_index = 0;
    undo_limit = default_undo_limit;
    has_quick_saved = false;

    physics_on = false;
    tracking_on = true;

    tile_id = Vector2i(0, 0);
    tile_flip_h = false;
    tile_flip_v = false;
    tile_flip_d = false;
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
        // Render on top of everything else
        CanvasLayer *ui_canvas = memnew(CanvasLayer);
        ui_canvas->set_layer(100);
        add_child(ui_canvas);

        // Load UI from scene file
        ui_scene = ResourceLoader::get_singleton()->load(ui_filepath, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        ui_node = ui_scene.ptr()->instantiate();
        ui_canvas->add_child(ui_node);

        // Connect Godot UI signals
        ui_node->connect("playtest_button_pressed", Callable(this, "toggle_physics"));
        
        ui_node->connect("debug_button_pressed", Callable(this, "_debug"));
        ui_node->connect("test_action_button_pressed", Callable(this, "_test_action"));
        ui_node->connect("undo_button_pressed", Callable(this, "undo_action"));
        ui_node->connect("redo_button_pressed", Callable(this, "redo_action"));

        ui_node->connect("load_level_path_selected", Callable(this, "load_new_level"));
        ui_node->connect("save_level_path_selected", Callable(this, "save_level"));
        ui_node->connect("quick_load_level_button_pressed", Callable(this, "quick_load_level"));
        ui_node->connect("quick_save_level_button_pressed", Callable(this, "quick_save_level"));
        ui_node->connect("unload_level_button_pressed", Callable(this, "unload_level"));
        ui_node->connect("reload_level_button_pressed", Callable(this, "reload_level"));

        ui_node->connect("tile_flip_button_pressed", Callable(this, "flip_tile"));
        ui_node->connect("tile_rotate_button_pressed", Callable(this, "rotate_tile"));
        ui_node->connect("tile_cycle_button_pressed", Callable(this, "cycle_tile"));

        ui_node->connect("add_tile_button_pressed", Callable(this, "add_tile"));
        ui_node->connect("remove_tile_button_pressed", Callable(this, "remove_tile"));
        ui_node->connect("add_player_button_pressed", Callable(this, "add_player"));
        ui_node->connect("add_enemy_button_pressed", Callable(this, "add_enemy"));
        ui_node->connect("add_entity_button_pressed", Callable(this, "add_entity"));
    }

    // Load default level
    load_level(level_filepath);
}

// - - - Action Handling Functions - - -

/**
 * @brief Track a new action and its reverse action.
 * 
 * @param new_action New action
 * @param reverse_action Reserse action
 * @return Variant Output of calling new_action
 */
void LevelEditorController::track_action(Callable redo_action, Callable undo_action) {
    // Remove actions in old timeline
    while (action_index < redo_stack->size()) {
        undo_stack->pop_back();
        redo_stack->pop_back();
    }

    // Track new action
    undo_stack->push_back(undo_action);
    redo_stack->push_back(redo_action);
    ++action_index;

    if (action_index > undo_limit) {
        // Remove oldest action
        undo_stack->pop_front();
        redo_stack->pop_front();
        --action_index;
    }
}

/**
 * @brief Clear all tracked undo/ redo actions.
 * 
 */
void LevelEditorController::clear_action() {
    undo_stack->clear();
    redo_stack->clear();
    action_index = 0;
    quick_save_action_index = 0;
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
    tracking_on = false;
    Variant output = action.call();
    tracking_on = true;
    return output;
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
    tracking_on = false;
    Variant output = action.call();
    tracking_on = true;
    return output;
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

void LevelEditorController::set_level_filepath(String filepath) {
    level_filepath = filepath;
}

void LevelEditorController::load_new_level(String filepath) {
    has_quick_saved = false;
    clear_action();
    load_level(filepath);
}

void LevelEditorController::load_level(String filepath) {
    if (ResourceLoader::get_singleton()->exists(filepath, "PackedScene")) {
        unload_level();
        level_filepath = BASE_FILEPATH(filepath);
        level_node = Level::import_level_tscn(filepath);
        level_node->set_process_mode(PROCESS_MODE_DISABLED); // Disable physics simulation
        physics_on = false;
        add_child(level_node);
    } else {
        UtilityFunctions::print("Warning: Cannot find level at '", filepath,"' !");
    }
}

void LevelEditorController::save_level(String filepath) {
    if ((is_level_loaded()) && (filepath.length() > 0)) {
        level_filepath = BASE_FILEPATH(filepath);
        Level::export_level_tscn(filepath, level_node);
    } else {
        UtilityFunctions::print("Warning: Nothing to save!");
    }
}

void LevelEditorController::quick_load_level() {
    if (has_quick_saved) {
        action_index = quick_save_action_index;
        load_level(QUICK_FILEPATH(level_filepath));
    }
}

void LevelEditorController::quick_save_level() {
    has_quick_saved = true;
    quick_save_action_index = action_index;
    save_level(QUICK_FILEPATH(level_filepath));
}

void LevelEditorController::unload_level() {
    if (is_level_loaded()) {
		level_node->queue_free();
		level_node = nullptr;
    }
}

void LevelEditorController::reload_level() {
    load_new_level(level_filepath);
}

bool LevelEditorController::is_level_loaded() {
    return (level_node != nullptr);
}

// - - - Level Editing Functions - - -

void LevelEditorController::toggle_physics() {
    physics_on = !physics_on;

    if (is_level_loaded()) {
        if (physics_on) {
            level_node->set_process_mode(PROCESS_MODE_ALWAYS);
        } else {
            level_node->set_process_mode(PROCESS_MODE_DISABLED);
        }
    }
}

Vector2i LevelEditorController::world_to_tile_pos(Vector2 world_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2i result;

    result.x = floor(world_pos.x / tile_map_scale.x / (real_t)tile_size.x);
    result.y = floor(world_pos.y / tile_map_scale.y / (real_t)tile_size.y);
    return result;
}

Vector2 LevelEditorController::tile_to_world_pos(Vector2i tile_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2 result;

    result.x = ((real_t)tile_pos.x * tile_map_scale.x * tile_size.x);
    result.y = ((real_t)tile_pos.y * tile_map_scale.y * tile_size.y);
    return result;
}

int LevelEditorController::get_tile_alt() {
    int tile_flags = 0;

    if (tile_flip_h) {
        tile_flags |= TileSetAtlasSource::TRANSFORM_FLIP_H;
    }
    if (tile_flip_v) {
        tile_flags |= TileSetAtlasSource::TRANSFORM_FLIP_V;
    }
    if (tile_flip_d) {
        tile_flags |= TileSetAtlasSource::TRANSFORM_TRANSPOSE;
    }

    return tile_flags;
}

void LevelEditorController::flip_tile(int mode) {
    switch (mode) {
    case 0:
        tile_flip_h = !tile_flip_h;
        break;
    case 1:
        tile_flip_v = !tile_flip_v;
        break;
    case 2:
        tile_flip_d = !tile_flip_d;
        break;
    }

    // Update UI
    emit_signal("tile_alt_changed", get_tile_alt());
}

void LevelEditorController::cycle_tile(bool next) {
    if (next) {
        tile_id.x = tile_id.x + 1;
        if (tile_id.x > 4) {tile_id.x = 0;}
    } else {
        tile_id.x = tile_id.x - 1;
        if (tile_id.x < 0) {tile_id.x = 4;}
    }

    // Update UI
    emit_signal("tile_type_changed", tile_id);
}

void LevelEditorController::rotate_tile(bool clockwise) {
    int rotation;

    // Encode
    if (!tile_flip_h && !tile_flip_v && !tile_flip_d) {
        rotation = 0;
    } else if (tile_flip_h && !tile_flip_v && tile_flip_d) {
        rotation = 1;
    } else if (tile_flip_h && tile_flip_v && !tile_flip_d) {
        rotation = 2;
    } else if (!tile_flip_h && tile_flip_v && tile_flip_d) {
        rotation = 3;
    } else if (tile_flip_h && !tile_flip_v && !tile_flip_d) {
        rotation = 4;
    } else if (tile_flip_h && tile_flip_v && tile_flip_d) {
        rotation = 5;
    } else if (!tile_flip_h && tile_flip_v && !tile_flip_d) {
        rotation = 6;
    } else if (!tile_flip_h && !tile_flip_v && tile_flip_d) {
        rotation = 7;
    }

    // Update
    if (clockwise) {
        if (rotation == 3) {
            rotation = 0; // 0-3
        } else if (rotation == 7) {
            rotation = 4; // 4-7
        } else {
            ++rotation;
        }
    } else {
        if (rotation == 0) {
            rotation = 3; // 0-3
        } else if (rotation == 4) {
            rotation = 7; // 4-7
        } else {
            --rotation;
        }
    }

    // Decode
    switch (rotation) {
    case 0:
        tile_flip_h = false; tile_flip_v = false; tile_flip_d = false;
        break;
    case 1:
        tile_flip_h = true; tile_flip_v = false; tile_flip_d = true;
        break;
    case 2:
        tile_flip_h = true; tile_flip_v = true; tile_flip_d = false;
        break;
    case 3:
        tile_flip_h = false; tile_flip_v = true; tile_flip_d = true;
        break;
    case 4:
        tile_flip_h = true; tile_flip_v = false; tile_flip_d = false;
        break;
    case 5:
        tile_flip_h = true; tile_flip_v = true; tile_flip_d = true;
        break;
    case 6:
        tile_flip_h = false; tile_flip_v = true; tile_flip_d = false;
        break;
    case 7:
        tile_flip_h = false; tile_flip_v = false; tile_flip_d = true;
        break;
    default: // == Case 0
        tile_flip_h = false; tile_flip_v = false; tile_flip_d = false;
        break;
    }

    // Update UI
    emit_signal("tile_alt_changed", get_tile_alt());
}

void LevelEditorController::add_tile(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get tile map
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
        if (!tile_map_layer) {
            UtilityFunctions::print("Warning: Tile map layer is missing!");
            return;
        }

        // Get tile set
        Ref<TileSet> tile_set = tile_map_layer->get_tile_set();
        if (tile_set.is_null()) {
            UtilityFunctions::print("Warning: Tile set is missing!");
            return;
        }

        // Calculate tile coordinates
        Vector2 tile_map_scale = tile_map_layer->get_scale();
        Vector2i tile_size = tile_set.ptr()->get_tile_size();
        Vector2i tile_pos = world_to_tile_pos(mouse_pos, tile_map_scale, tile_size);
        
        // Add tile (reversible)
        replace_tile(tile_pos, 0, tile_id, get_tile_alt());
    }
}

void LevelEditorController::remove_tile(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get tile map
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
        if (!tile_map_layer) {
            UtilityFunctions::print("Warning: Tile map layer is missing!");
            return;
        }

        // Get tile set
        Ref<TileSet> tile_set = tile_map_layer->get_tile_set();
        if (tile_set.is_null()) {
            UtilityFunctions::print("Warning: Tile set is missing!");
            return;
        }

        // Calculate tile coordinates
        Vector2 tile_map_scale = tile_map_layer->get_scale();
        Vector2i tile_size = tile_set.ptr()->get_tile_size();
        Vector2i tile_pos = world_to_tile_pos(mouse_pos, tile_map_scale, tile_size);
        
        // Remove tile (reversible)
        replace_tile(tile_pos, -1, Vector2i(-1, -1), -1);
    }
}

void LevelEditorController::replace_tile(Vector2i tile_pos, int source_id, Vector2i tile_id, int tile_alt) {
    if (is_level_loaded()) {
        // Get tile map
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
        if (!tile_map_layer) {
            UtilityFunctions::print("Warning: Tile map layer is missing!");
            return;
        }

        // Store the original tile information
        int old_source_id = tile_map_layer->get_cell_source_id(tile_pos);
        Vector2i old_tile_id = tile_map_layer->get_cell_atlas_coords(tile_pos);
        int old_tile_flags = tile_map_layer->get_cell_alternative_tile(tile_pos);

        // Replace with new tile
        tile_map_layer->set_cell(tile_pos, source_id, tile_id, tile_alt);
        
        // Track its reversible actions
        if (tracking_on) {
            track_action(
                Callable(this, "replace_tile").bind(tile_pos, source_id, tile_id, tile_alt), 
                Callable(this, "replace_tile").bind(tile_pos, old_source_id, old_tile_id, old_tile_flags)
            );
        }
    }
}

void LevelEditorController::add_scene_object(Vector2 mouse_pos, String list_name, String scene_path) {
    if (is_level_loaded()) {
        // Get tile map
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
        if (!tile_map_layer) {
            UtilityFunctions::print("Warning: Tile map layer is missing!");
            return;
        }

        // Gte tile set
        Ref<TileSet> tile_set = tile_map_layer->get_tile_set();
        if (tile_set.is_null()) {
            UtilityFunctions::print("Warning: Tile set is missing!");
            return;
        }

        // Calculate tile coordinates
        Vector2 tile_map_scale = tile_map_layer->get_scale();
        Vector2i tile_size = tile_set.ptr()->get_tile_size();
        Vector2i tile_pos = world_to_tile_pos(mouse_pos, tile_map_scale, tile_size);

        // Calculate object coordinates
        Vector2 offset = (Vector2)tile_size * tile_map_scale * 0.5; // Offset from tile top-left corner to center
        Vector2 object_pos = tile_to_world_pos(tile_pos, tile_map_scale, tile_size) + offset; // Object origin is at its center, not its top-left corner

        // Generate object
        Ref<PackedScene> object_scene = ResourceLoader::get_singleton()->load(scene_path, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        RigidBody2D * object_node = (RigidBody2D *)(object_scene.ptr()->instantiate());
        object_node->set_position(object_pos);

        // Add object
        level_node->add_node_to_list(list_name, object_node);
    }
}

void LevelEditorController::add_player(Vector2 mouse_pos) {
    // TODO hard-coded to add the yinyang orb
    if (is_level_loaded()) {
        add_scene_object(mouse_pos, level_node->player_list_name, "res://entity/yinyang_orb.tscn");
    }
}

void LevelEditorController::add_enemy(Vector2 mouse_pos) {
    // TODO hard-coded to add the yinyang orb
    if (is_level_loaded()) {
        add_scene_object(mouse_pos, level_node->enemy_list_name, "res://entity/yinyang_orb.tscn");
    }
}

void LevelEditorController::add_entity(Vector2 mouse_pos) {
    // TODO hard-coded to add the yinyang orb
    if (is_level_loaded()) {
        add_scene_object(mouse_pos, level_node->entity_list_name, "res://entity/yinyang_orb.tscn");
    }
}

// - - - Test Functions - - -

/**
 * @brief Test function (reversible).
 * 
 * @param n Integer
 */
void LevelEditorController::_test_action(String s, int n) {
    UtilityFunctions::print(s, ": ", n);

    // Track its reversible actions
    if (tracking_on) {
        track_action(
            Callable(this, "_test_action").bind("Redo", n), 
            Callable(this, "_test_action").bind("Undo", n)
        );
    }
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
        Callable c = (*undo_stack)[i];
        UtilityFunctions::print(c.get_method(), ":", c.get_bound_arguments());
    }

    UtilityFunctions::print("Redo Stack:");
    for (int i = 0; i < redo_stack->size(); ++i) {
        Callable c = (*redo_stack)[i];
        UtilityFunctions::print(c.get_method(), ":", c.get_bound_arguments());
    }

    UtilityFunctions::print("Action Index:", action_index);
    UtilityFunctions::print("Quicksave's Action Index:", quick_save_action_index);

    UtilityFunctions::print("Undo Limit:", undo_limit);

    UtilityFunctions::print("------------------------------");
}