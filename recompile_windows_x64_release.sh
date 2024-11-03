#!/bin/sh

# Author: Chun Ho Chan (Edward)
# Last Modified: 2024-09-14

# Auto-compile all the C++ classes with GDExtension
echo "Compiling GDExtension modules for 64-bit Windows, Godot's Release Mode..."
echo ""
scons platform=windows target=template_release
echo ""
echo "Done! You may open Godot and run/ edit your project."
echo "Press [Enter] to close."

# Keep terminal open and wait for input
read input
