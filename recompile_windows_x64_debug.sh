#!/bin/bash

<< 'COMMENT'
Description:
    This Bash script compiles GDExtension modules for 64-bit Windows, Debug mode.
Author:
    Chun Ho Chan (Edward)
Version:
    v1.1 (2024-11-04)
License:
    GNU Affero General Public License v3.0
COMMENT

printf "> Executing Bash script ...\n"
printf "> Setting: 64-bit Windows, Debug mode\n"

input='r'
while [[ $input == [rR] ]]
do
    printf "\n> Generating 'register_types.cpp' with Python ...\n"
    python generate_register_types_cpp.py
    printf "Done. (exit code: %d)\n" $?

    printf "\n> Compiling GDExtension modules with SCon ...\n"
    scons platform=windows target=template_debug debug_symbols=yes
    printf "Done. (exit code: %d)\n" $?

    printf "\n> Compilation finished. You may open Godot and run/ edit your project.\n"
    read -p "> Press 'q' to exit, 'r' to recompile: " -n 1 input
    printf "\n\n> ------------------------------\n"
done