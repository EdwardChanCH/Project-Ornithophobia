/**
 * @file level_editor_controller.cpp
 * @author Chun Ho Chan (Edward)
 * @brief Mediator Pattern (MVC).
 * This class controlls all communication between Godot UI (view) and Level data (model).
 * @version 0.3.1
 * @date 2024-11-25
 * 
 */

// Convert a save path to the base path.
#define BASE_FILEPATH(filepath) \
filepath.replace("_quicksave", "")

// Convert a save path to the quicksave path.
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
    ADD_SIGNAL(MethodInfo("level_loaded", PropertyInfo(Variant::STRING, "filepath")));
    ADD_SIGNAL(MethodInfo("level_saved", PropertyInfo(Variant::STRING, "filepath")));
    ADD_SIGNAL(MethodInfo("tile_type_changed", PropertyInfo(Variant::VECTOR2I, "atlas_coords")));
    ADD_SIGNAL(MethodInfo("tile_alt_changed", PropertyInfo(Variant::INT, "tile_alt")));
    ADD_SIGNAL(MethodInfo("debug_message", PropertyInfo(Variant::STRING, "message")));
    ADD_SIGNAL(MethodInfo("normal_message", PropertyInfo(Variant::STRING, "message")));
    ADD_SIGNAL(MethodInfo("warning_message", PropertyInfo(Variant::STRING, "message")));
    ADD_SIGNAL(MethodInfo("error_message", PropertyInfo(Variant::STRING, "message")));

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

    ClassDB::bind_method(D_METHOD("set_physics", "active"), &LevelEditorController::set_physics);
    ClassDB::bind_method(D_METHOD("world_to_tile_pos", "world_pos", "tile_map_scale", "tile_size"), &LevelEditorController::world_to_tile_pos);
    ClassDB::bind_method(D_METHOD("tile_to_world_pos", "tile_pos", "tile_map_scale", "tile_size"), &LevelEditorController::tile_to_world_pos);
    ClassDB::bind_method(D_METHOD("cursor_to_tile_pos", "cursor_pos"), &LevelEditorController::cursor_to_tile_pos);
    ClassDB::bind_method(D_METHOD("cursor_to_object_pos", "cursor_pos", "list_name"), &LevelEditorController::cursor_to_object_pos);
    ClassDB::bind_method(D_METHOD("get_tile_alt"), &LevelEditorController::get_tile_alt);
    
    ClassDB::bind_method(D_METHOD("flip_tile", "mode"), &LevelEditorController::flip_tile);
    ClassDB::bind_method(D_METHOD("rotate_tile", "clockwise"), &LevelEditorController::rotate_tile);
    ClassDB::bind_method(D_METHOD("cycle_tile", "next"), &LevelEditorController::cycle_tile);

    ClassDB::bind_method(D_METHOD("replace_tile", "tile_pos", "source_id", "tile_id", "tile_alt"), &LevelEditorController::replace_tile);
    ClassDB::bind_method(D_METHOD("add_scene_object", "object_pos", "list_name", "scene_path"), &LevelEditorController::add_scene_object);
    ClassDB::bind_method(D_METHOD("remove_scene_object", "object_pos", "list_name", "scene_path"), &LevelEditorController::remove_scene_object);
    ClassDB::bind_method(D_METHOD("add_tile", "mouse_pos"), &LevelEditorController::add_tile);
    ClassDB::bind_method(D_METHOD("remove_tile", "mouse_pos"), &LevelEditorController::remove_tile);
    ClassDB::bind_method(D_METHOD("add_player", "mouse_pos"), &LevelEditorController::add_player);
    ClassDB::bind_method(D_METHOD("remove_player", "mouse_pos"), &LevelEditorController::remove_player);
    ClassDB::bind_method(D_METHOD("add_enemy", "mouse_pos"), &LevelEditorController::add_enemy);
    ClassDB::bind_method(D_METHOD("remove_enemy", "mouse_pos"), &LevelEditorController::remove_enemy);
    ClassDB::bind_method(D_METHOD("add_entity", "mouse_pos"), &LevelEditorController::add_entity);
    ClassDB::bind_method(D_METHOD("remove_entity", "mouse_pos"), &LevelEditorController::remove_entity);

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
        ui_node->connect("physics_button_toggled", Callable(this, "set_physics"));
        
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
        ui_node->connect("remove_player_button_pressed", Callable(this, "remove_player"));
        ui_node->connect("add_enemy_button_pressed", Callable(this, "add_enemy"));
        ui_node->connect("remove_enemy_button_pressed", Callable(this, "remove_enemy"));
        ui_node->connect("add_entity_button_pressed", Callable(this, "add_entity"));
        ui_node->connect("remove_entity_button_pressed", Callable(this, "remove_entity"));

        ui_node->connect("debug_button_pressed", Callable(this, "_debug"));
        ui_node->connect("test_action_button_pressed", Callable(this, "_test_action"));
    }

    // Load default level
    load_level(level_filepath);
}

// - - - Settings Functions - - -

/**
 * @brief Toggle physics.
 * 
 */
void LevelEditorController::set_physics(bool active) {
    if (is_level_loaded()) {
        level_node->set_physics(active);
    }
}

// - - - Action Handling Functions - - -

/**
 * @brief Track a redo action and an undo action.
 * 
 * @param redo_action Redo action
 * @param undo_action Undo action
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
        emit_signal("warning_message", "No undo available.");
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
        emit_signal("warning_message", "No redo available.");
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

/**
 * @brief Set a level filepath. 
 * i.e. Overwrite the default level filepath.
 * 
 * @param filepath Filepath
 */
void LevelEditorController::set_level_filepath(String filepath) {
    level_filepath = filepath;
}

/**
 * @brief Load a new level. 
 * Reset action history and quicksave status. 
 * 
 * @param filepath Filepath
 */
void LevelEditorController::load_new_level(String filepath) {
    has_quick_saved = false;
    clear_action();
    load_level(filepath);
}

/**
 * @brief Load a level from file.
 * 
 * @param filepath Filepath
 */
void LevelEditorController::load_level(String filepath) {
    if (ResourceLoader::get_singleton()->exists(filepath, "PackedScene")) {
        unload_level();
        level_filepath = BASE_FILEPATH(filepath);
        level_node = Level::import_level_tscn(filepath);
        level_node->set_physics(false); // Disable physics simulation
        add_child(level_node);
        emit_signal("level_loaded", filepath);
    } else {
        emit_signal("warning_message", "Cannot find level at '" + filepath + "' !");
    }
}

/**
 * @brief Save a level to file.
 * 
 * @param filepath Filepath
 */
void LevelEditorController::save_level(String filepath) {
    if ((is_level_loaded()) && (filepath.length() > 0)) {
        // Check if the level is valid
        if (level_node->get_list_length(level_node->player_list_name) < 1) {
            emit_signal("error_message", "Save aborted. Cannot have less than 1 player.");
            return;
        }
        if (level_node->get_list_length(level_node->player_list_name) > 1) {
            emit_signal("error_message", "Save aborted. Cannot have more than 1 player.");
            return;
        }
        if (level_node->get_list_length(level_node->enemy_list_name) < 1) {
            emit_signal("error_message", "Save aborted. Cannot have less than 1 enemy.");
            return;
        }

        // Change save location to user data
        filepath = filepath.replace("res://", "user://");

        // Save level as TSCN
        level_filepath = BASE_FILEPATH(filepath);
        level_node->set_physics(false); // Disable physics simulation
        Level::export_level_tscn(filepath, level_node);

        // Update thumbnail
        emit_signal("level_saved", filepath);
    } else {
        emit_signal("warning_message", "Nothing to save!");
    }
}

/**
 * @brief Quick load a level from file.
 * 
 */
void LevelEditorController::quick_load_level() {
    if (has_quick_saved) {
        action_index = quick_save_action_index;
        load_level(QUICK_FILEPATH(level_filepath.replace("res://", "user://")));
    } else {
        emit_signal("warning_message", "Cannot quick load before quick saving at least once!");
    }
}

/**
 * @brief Quick save a level to file.
 * 
 */
void LevelEditorController::quick_save_level() {
    has_quick_saved = true;
    quick_save_action_index = action_index;
    save_level(QUICK_FILEPATH(level_filepath.replace("res://", "user://")));
}

/**
 * @brief Unload a level.
 * 
 */
void LevelEditorController::unload_level() {
    if (is_level_loaded()) {
		level_node->queue_free();
		level_node = nullptr;
    }
}

/**
 * @brief Reload the last opened level from file.
 * 
 */
void LevelEditorController::reload_level() {
    load_new_level(level_filepath);
}

/**
 * @brief Check if any level is loaded.
 * 
 * @return true 
 * @return false 
 */
bool LevelEditorController::is_level_loaded() {
    return (level_node != nullptr);
}

// - - - Level Editing Functions - - -

/**
 * @brief Convert a global position to a tile position.
 * 
 * @param world_pos Global Position
 * @param tile_map_scale Tile Map Scale
 * @param tile_size Tile Size
 * @return Vector2i Tile Position
 */
Vector2i LevelEditorController::world_to_tile_pos(Vector2 world_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2i result;

    result.x = floor(world_pos.x / tile_map_scale.x / (real_t)tile_size.x);
    result.y = floor(world_pos.y / tile_map_scale.y / (real_t)tile_size.y);
    return result;
}

/**
 * @brief Convert a tile position to a global position.
 * 
 * @param tile_pos Tile Position
 * @param tile_map_scale Tile Map Scale
 * @param tile_size Tile Size
 * @return Vector2 Tile Position
 */
Vector2 LevelEditorController::tile_to_world_pos(Vector2i tile_pos, Vector2 tile_map_scale, Vector2i tile_size) {
    Vector2 result;

    result.x = ((real_t)tile_pos.x * tile_map_scale.x * tile_size.x);
    result.y = ((real_t)tile_pos.y * tile_map_scale.y * tile_size.y);
    return result;
}

/**
 * @brief Convert a global cursor position to a tile position.
 * 
 * @param cursor_pos Cursor Position
 * @return Vector2i Tile Position
 */
Vector2i LevelEditorController::cursor_to_tile_pos(Vector2 cursor_pos) {
    // Get tile map
    TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
    if (!tile_map_layer) {
        emit_signal("warning_message", "Tile map layer is missing!");
        return Vector2i(0 ,0);
    }

    // Get tile set
    Ref<TileSet> tile_set = tile_map_layer->get_tile_set();
    if (tile_set.is_null()) {
        emit_signal("warning_message", "Tile set is missing!");
        return Vector2i(0 ,0);
    }

    // Calculate tile coordinates
    Vector2 tile_map_scale = tile_map_layer->get_scale();
    Vector2i tile_size = tile_set.ptr()->get_tile_size();
    Vector2i tile_pos = world_to_tile_pos(cursor_pos, tile_map_scale, tile_size);

    return tile_pos;
}

/**
 * @brief Convert a global cursor position to an global object position.
 * 
 * @param cursor_pos Cursor Position
 * @return Vector2 Object Position
 */
Vector2 LevelEditorController::cursor_to_object_pos(Vector2 cursor_pos, String list_name) {
    // Get tile map
    TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
    if (!tile_map_layer) {
        emit_signal("warning_message", "Tile map layer is missing!");
        return Vector2(0, 0);
    }

    // Get tile set
    Ref<TileSet> tile_set = tile_map_layer->get_tile_set();
    if (tile_set.is_null()) {
        emit_signal("warning_message", "Tile set is missing!");
        return Vector2(0, 0);
    }

    // Calculate tile coordinates
    Vector2 tile_map_scale = tile_map_layer->get_scale();
    Vector2i tile_size = tile_set.ptr()->get_tile_size();
    Vector2i tile_pos = world_to_tile_pos(cursor_pos, tile_map_scale, tile_size);

    // Calculate object coordinates
    Vector2 offset = (Vector2)tile_size * tile_map_scale * 0.5; // Offset from tile top-left corner to center
    Vector2 object_pos = tile_to_world_pos(tile_pos, tile_map_scale, tile_size) + offset; // Object origin is at its center, not its top-left corner
    if (list_name == level_node->player_list_name) {
        object_pos += Vector2(0, -24);
    }

    return object_pos;
}

/**
 * @brief Get the alternative tile setting.
 * 
 * @return int Alternative Tile Setting
 */
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

/**
 * @brief Flip a tile.
 * 
 * @param mode Mode: 0 = Horizontal, 1 = Vertical, 2 = Diagonal
 */
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

/**
 * @brief Cycle tile type.
 * 
 * @param next Next/ Previous
 */
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

/**
 * @brief Rotate a tile.
 * 
 * @param clockwise Clockwise/ Counter-Clockwise
 */
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

/**
 * @brief Replace a tile. 
 * Note: This action is reversible.
 * 
 * @param tile_pos Tile Position
 * @param source_id Source ID (usually 0)
 * @param tile_id Tile ID
 * @param tile_alt Alternative Tile Setting
 */
void LevelEditorController::replace_tile(Vector2i tile_pos, int source_id, Vector2i tile_id, int tile_alt) {
    if (is_level_loaded()) {
        // Get tile map
        TileMapLayer * tile_map_layer = (TileMapLayer *)(level_node->get_node_in_list(level_node->tile_list_name, "Terrain"));
        if (!tile_map_layer) {
            emit_signal("warning_message", "Tile map layer is missing!");
            return;
        }

        // Hold the original tile information
        int old_source_id = tile_map_layer->get_cell_source_id(tile_pos);
        Vector2i old_tile_id = tile_map_layer->get_cell_atlas_coords(tile_pos);
        int old_tile_alt = tile_map_layer->get_cell_alternative_tile(tile_pos);

        // Check if there is any difference
        if ((source_id == old_source_id) && (tile_id == old_tile_id) && (tile_alt == old_tile_alt)) {
            // emit_signal("warning_message", "No difference between new and old tiles!");
            return;
        }

        // Replace with new tile
        tile_map_layer->set_cell(tile_pos, source_id, tile_id, tile_alt);
        
        // Track its reversible actions
        if (tracking_on) {
            track_action(
                Callable(this, "replace_tile").bind(tile_pos, source_id, tile_id, tile_alt), 
                Callable(this, "replace_tile").bind(tile_pos, old_source_id, old_tile_id, old_tile_alt)
            );
        }
    }
}

void LevelEditorController::add_scene_object(Vector2 object_pos, String list_name, String scene_path) {
    if (is_level_loaded()) {
        // Generate object
        Ref<PackedScene> object_scene = ResourceLoader::get_singleton()->load(scene_path, "PackedScene", ResourceLoader::CACHE_MODE_REUSE);
        Node2D *object_node = (Node2D *)(object_scene.ptr()->instantiate());
        object_node->set_position(object_pos);
        object_node->set_process_mode(Node::PROCESS_MODE_INHERIT);

        // Add object
        level_node->add_node_to_list(list_name, object_node);

        // Track its reversible actions
        if (tracking_on) {
            track_action(
                Callable(this, "add_scene_object").bind(object_pos, list_name, scene_path), 
                Callable(this, "remove_scene_object").bind(object_pos, list_name, scene_path)
            );
        }
    }
}

void LevelEditorController::remove_scene_object(Vector2 object_pos, String list_name, String scene_path) {
    if (is_level_loaded()) {
        // Find object in list
        int num_children = level_node->get_list_length(list_name);
        Node2D *child_node;
        for (int i = num_children - 1; i >= 0; --i) {
            child_node = (Node2D *)level_node->get_node_in_list_by_index(list_name, i);
            if (!child_node) {
                continue;
            }

            // Check if instance type matches
            if (child_node->get_scene_file_path() != scene_path) {
                continue;
            }

            // Checek if global coordinates matches
            if (child_node->get_position() != object_pos) {
                continue;
            }

            // Remove object
            child_node->queue_free();

            // Track its reversible actions
            if (tracking_on) {
                track_action(
                    Callable(this, "remove_scene_object").bind(object_pos, list_name, scene_path), 
                    Callable(this, "add_scene_object").bind(object_pos, list_name, scene_path)
                );
            }
            return;
        }

        // Cannot find object
        // emit_signal("warning_message", "Cannot find object of the same type!");
        return;
    }
}

/**
 * @brief Add a tile.
 * 
 * @param mouse_pos Global Cursor Position
 */
void LevelEditorController::add_tile(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get tile position
        Vector2i tile_pos = cursor_to_tile_pos(mouse_pos);
        
        // Add tile (reversible)
        replace_tile(tile_pos, 0, tile_id, get_tile_alt());
    }
}

/**
 * @brief Remove a tile.
 * 
 * @param mouse_pos Global Cursor Position
 */
void LevelEditorController::remove_tile(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get tile position
        Vector2i tile_pos = cursor_to_tile_pos(mouse_pos);
        
        // Remove tile (reversible)
        replace_tile(tile_pos, -1, Vector2i(-1, -1), -1);
    }
}

void LevelEditorController::add_player(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->player_list_name);

        // Add player
        add_scene_object(object_pos, level_node->player_list_name, "res://object/player_controller.tscn");
    }
}

void LevelEditorController::remove_player(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->player_list_name);

        // Remove player
        remove_scene_object(object_pos, level_node->player_list_name, "res://object/player_controller.tscn");
    }
}

void LevelEditorController::add_enemy(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->enemy_list_name);

        // Add enemy
        add_scene_object(object_pos, level_node->enemy_list_name, "res://object/enemy_controller.tscn");
    }
}

void LevelEditorController::remove_enemy(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->enemy_list_name);

        // Remove enemy
        remove_scene_object(object_pos, level_node->enemy_list_name, "res://object/enemy_controller.tscn");
    }
}

void LevelEditorController::add_entity(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->entity_list_name);

        // Add entity
        add_scene_object(object_pos, level_node->entity_list_name, "res://entity/yinyang_orb.tscn");
    }
}

void LevelEditorController::remove_entity(Vector2 mouse_pos) {
    if (is_level_loaded()) {
        // Get object position
        Vector2 object_pos = cursor_to_object_pos(mouse_pos, level_node->entity_list_name);

        // Remove entity
        remove_scene_object(object_pos, level_node->entity_list_name, "res://entity/yinyang_orb.tscn");
    }
}

// - - - Test Functions - - -

/**
 * @brief Test function, prints a message with a number.
 * Note: This action is reversible.
 * 
 * @param s Message
 * @param n Number
 */
void LevelEditorController::_test_action(String s, int n) {
    emit_signal("normal_message", s + ": " + UtilityFunctions::str(n));

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
    String message = "";
    
    message += "------------------------------\n";
    message += UtilityFunctions::str("Class: ", "LevelEditorController", "\n");
    message += "Undo Stack:\n";
    for (int i = 0; i < undo_stack->size(); ++i) {
        Callable c = (*undo_stack)[i];
        message += UtilityFunctions::str("    ", c.get_method(), ": ", c.get_bound_arguments(), "\n");
    }
    message += "Redo Stack:\n";
    for (int i = 0; i < redo_stack->size(); ++i) {
        Callable c = (*redo_stack)[i];
        message += UtilityFunctions::str("    ", c.get_method(), ": ", c.get_bound_arguments(), "\n");
    }
    message += UtilityFunctions::str("Action Index: ", action_index, "\n");
    message += UtilityFunctions::str("Quicksave's Action Index: ", quick_save_action_index, "\n");
    message += UtilityFunctions::str("Undo Limit: ", undo_limit, "\n");
    message += "------------------------------";

    emit_signal("debug_message", message);
}