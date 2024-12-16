/**
 * @file test_main.cpp
 * @author Chun Ho Chan (Edward)
 * @brief This program launches Google Test.
 * Note: It does not work.
 * @version 0.1.0
 * @date 2024-11-15
 * 
 */

#include <gtest/gtest.h> // Google Test
#include <gmock/gmock.h> // Google Mock

#include <godot_cpp/core/class_db.hpp> // This hold the list of all registered classes
#include <godot_cpp/classes/node2d.hpp> // Bindings to the parent class
#include "boilerplate_macros.h" // Macros for GDExtension's boilerplate code
#include "globals.h" // Godot's built-in classes

// Everything in GDExtension is defined within the namespace "godot"
using namespace godot;

int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Example, Test1) {
    printf("Some Godot Variant types work as expected.\n");

    auto result1 = Vector2(1, 2); // ok
    
    auto result2 = Dictionary(); // crash

    EXPECT_EQ(Vector2(1, 2), result1); // pass
}

TEST(Example, Test2) {
    printf("Godot Object/ Node types will crash upon initialization.\n");

    Node *result1 = nullptr; // ok
    result1 = memnew(Node()); // crash

    auto result2 = Node(); // crash

    EXPECT_EQ(true, true); // not executed
}