/**
 * \page readme-1 README (src/)
 */
# README File

* Author:  
  * Chun Ho Chan, Edward  
* Last Modified:  
  * 2024-09-12  

## Important Notes
* When coding a new Godot class, be sure to add *# include "classname.h"* and *GDREGISTER_CLASS(ClassName);* in the ".../3307GroupProject/src/*register_types.cpp*" file.  
* When renaming the *cs3307_library_init()* function, be sure to also update its name stored in the ".../3307GroupProject/game/bin/*cs3307.gdextension*" file.  
* Renaming any class names would break the corresponding nodes used in Godot scene files (.tscn). 
* To fix a broken scene file, open it in a text editor, and manually update all the old class names/ signal names inside.  