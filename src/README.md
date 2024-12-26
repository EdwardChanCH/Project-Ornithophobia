# README File (1)

* Author:  
  * Chun Ho Chan, Edward  
* Last Modified:  
  * 2024-09-12  

## Important Notes

* When coding a new Godot class, be sure to add *# include "classname.h"* and *GDREGISTER_CLASS(ClassName);* in the "{root}/src/*register_types.cpp*" file.  
* When renaming the *cs3307_library_init()* function, be sure to also update its name stored in the "{root}/game/bin/*cs3307.gdextension*" file.  
* Renaming any C++ class will break the corresponding nodes in the scene files (.tscn). This is fixable by manually changing the class of each broken node.  
* To fix a broken scene file, open it in a text editor and manually update all the old class names/ signal names inside.  
