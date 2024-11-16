#!/bin/bash

<< 'COMMENT'
Description:
    This Bash script launches "generate_custom_class_h_cpp.py".
Author:
    Chun Ho Chan (Edward)
Version:
    v1.0 (2024-11-11)
License:
    GNU Affero General Public License v3.0
COMMENT

printf "> Generating 'custom_class.h', 'custom_class.cpp' with Python...\n"
python generate_custom_class_h_cpp.py
printf "Done. (exit code: %d)\n" $?

read -p "> Press 'Enter' to exit." -n 1 input