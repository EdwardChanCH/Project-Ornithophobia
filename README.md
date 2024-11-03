# README File

## 0. Copyright & Credits

### a. Team Members:

* *Full Name*  
  * *Email Address*  
* Jacob Couture  
  * jcoutur9@uwo.ca  
* Chun Ho Chan (Edward)  
  * ccha232.uwo.ca  
* Sameer Mehboob Bugti  
  * sbugti@uwo.ca  

### b. Course Term:

Fall 2024  

### c. Course Info:

This game project was created as part of CompSci 3307A at Western University.  

### d. 3rd Party Resources

* *Media Type (copyright license)*  
  * *"Source Title", creator*  
    * *Source URL*  

## 1. Short Description

This is a game project (combined individual assignments) of CompSci 3307A at Western University.  

## 2. Required Libraries & 3rd Party Tools

* *Library/Tool*  
  * *Version Number*  
* Godot  
  * version 4.3  
  * Note: Use the standard version of Godot instead of the .NET version.  
* GDExtension C++ (godot-cpp)  
  * version 4.3  
  * Note: Use the same version as Godot.  
* C++ Compiler (MinGW)  
  * version 13.2.0+  
  * Note: This compiler must be the same as the one that compiled Godot from source (MinGW for official releases).  
  * Note: If you self-compiled Godot from source using MSVC, then you must also use MSVC to compile this project.  
* Python  
  * version 3.12.4+  
  * Note: This is required for using SCons.  
* SCons  
  * version 4.8.1+ (PyPI)  
  * Note: This is GDExtension's suggested build tool.  

## 3. How To Compile (step-by-step guide)

Setting Up SCons (one-time setup):  
1. Open a terminal in the ".../3307GroupProject/*godot-cpp*/" directory, and then:  
  * Run the command "scons platform=<platform> custom_api_file=<PATH_TO_FILE> <bits=64>".  
  * Example: scons platform=windows custom_api_file="./gdextension/extension_api.json" bits=64  
  * (Full Documentation: https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html)  
2. Wait about 5 minutes for it to finish.  

Steps:  
1. (one-time setup) Check the "*SConstruct*" file to see if the build path is correct (in each *env.SharedLibrary(...)* function's parameter).  
2. Open a terminal in the ".../*3307GroupProject*/" directory, and then either:  
  * Run the command "scons platform=windows target=template_debug debug_symbols=yes" to compile debug builds (support hot-reload), OR,  
  * Run the command "scons platform=windows target=template_release" to compile release builds (more optimized).  
  * (Note: Alternatively, you may run one of the included shell scripts for convenience.)  
3. Open the project in Godot editor.  
4. (Project > Export) Export the project in Godot editor.  
  * (Note: You may need to download & install a template first.)  

Note:  
* The included build is compiled for 64-bit Windows, Godot's Debug Mode.  
* When renaming the directory ".../3307GroupProject/*game*/", be sure to also update its path stored in the file ".../*SConstruct*", and also re-import the project in Godot editor.  

## 4. How To Run (step-by-step guide)

Steps:  
1. Go to the ".../3307GroupProject/*export*/" directory.  
2. Depending on your operating system, go to one of the subdirectories: "*windows*", "*macos*", or "*linux*".  
3. Double click on the "*UntitledShotgunProject.exe*" executable to run it.  

Alternatively:  
1. Compile the game from source (refer to Section 3).  
2. Import the ".../3307GroupProject/*game*/" directory into Godot, and then exit Godot.  
3. Reopen Godot, highlight the project "*UntitledShotgunProject*", and click "Run".  
  * (Note: If Godot fails to run, check the Godot executable path stored in the file ".../game/.godot/editor/*project_metadata.cfg*".)  

## 5. User Guide

All of the necessary information can be found within the game.  

## 6. Account Usernames & Passwords

This game does not use an account system. It behaves like an arcade game which only a username is required to play.  

## 7. Cheat Mode Activation

This game does not have a cheat mode (or does it?).  

## 8. Developer Mode Activation

Compile and run Godot in Debug mode.  

## 9. Notes For TA

**Thank you for playing our game!**  

## 10. Miscellaneous

List of other README files:  
* ".../3307GroupProject/README.md"  
* ".../3307GroupProject/src/README.md"  
* ".../3307GroupProject/game/bin/README.md"  

This Godot project was created with these settings: 
* Renderer: "Compatibility" mode  
  * Supports desktop, mobile + web platforms.  
  * Least advanced 3D graphics (currently WIP).  
  * Intended for low-end/ older devices.  
  * Uses OpenGL 3 backend (OpenGL 3.3/ ES 3.0/ WebGL2).  
  * Fastest rendering for simple scenes.  
  * (Note: The renderer can be changed later, but scenes may need to be adjusted.)  
* Version Control Metadata: Git  

Documenting Custom C++ Classes:  
1. Open a terminal in the ".../3307GroupProject/game/" directory, and then:  
  * Run the command "<path_to_godot> --doctool ../ --gdextension-docs"  
  * Example: "C:/Program Files/Godot/Godot_v4.3-stable/Godot_v4.3-stable_win64.exe" --doctool ../ --gdextension-docs  
  * (Note: For PowerShell users, add an & character at the front if your path to godot is surrounded by "".)  
  * (Full Documentation: https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_docs_system.html)  
2. Press Enter when the message "Deleting docs cache..." appears.  
3. Check if new .XML documentation files are located in the ".../3307GroupProject/doc_classes/" directory.  

Fixing VSCode C++ Undetected Header Files:  
1. Open VSCode and open the Extension Settings page of "C/C++".  
2. Search for the "Include Path" setting, and then add item:  
        ${workspaceFolder}/**  
3. At the top navigation bar, click "File", and "Add Folder to Workplace".  
4. Select the ".../3307GroupProject/export/" directory.  
5. At the top navigation bar, click "File", and "Save Workplace As".  
6. Save the workplace file next to the ".../3307GroupProject/" directory.  
7. Restart VSCode and reopen the workplace.  