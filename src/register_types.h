/**
 * @file register_types.h
 * @author GDExtension
 * @brief Header file.
 * DO NOT RENAME THIS FILE!
 * @version 1.0.0
 * @date 2024-11-10
 * 
 */

#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes

using namespace godot; // Required by GDExtension

void initialize_custom_module(ModuleInitializationLevel p_level); // Free function
void uninitialize_custom_module(ModuleInitializationLevel p_level); // Free function

#endif