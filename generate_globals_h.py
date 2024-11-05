"""
Description:
    This Python script generates 'globals.h' in './src/'.
Author:
    Chun Ho Chan (Edward)
Version:
    v1.1 (2024-11-04)
License:
    GNU Affero General Public License v3.0
"""

import os

def list_subdirectories(directory):
    """
    Return a list of sub-directories, including the input directory.
    """
    return [os.path.relpath(root).replace("\\", "/") + "/" for (root, dirs, files) in os.walk(directory)]

def list_files(directory, file_extension):
    """
    Return a list of files with the file extension located inside the input directory.
    """
    return [f for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f)) and f.endswith(file_extension)]

def include_setting(directory):
    """
    Return 1 if the headers in the input directory should be included by default, 0 if otherwise.
    """
    WHITELIST = [
        "godot-cpp/gen/include/godot_cpp/classes/", 
        "godot-cpp/gen/include/godot_cpp/variant/", 
        "godot-cpp/include/godot_cpp/variant/"
    ]
    return int(directory in WHITELIST)

def main():
    """
    Main method.
    """
    # Generate a list of all directories
    subdirectories = list_subdirectories(".\\godot-cpp")
    
    # Write to the header file
    with open(".\\src\\globals.h", mode="w", encoding="utf-8") as output:
        # Write basic file information
        output.write("// Automatically generated with \"recompile_globals_h.py\".\n")
        output.write("// This file (selectively) includes all the Godot class headers provided by GDExtension.\n")
        output.write("// 0 = Inactive.\n")
        output.write("// 1 = Active.\n")
        output.write("\n")

        # Write include guards, at the start of file
        output.write("#ifndef GLOBALS_H\n")
        output.write("#define GLOBALS_H\n")
        output.write("\n")

        # Search each directory
        for d in subdirectories:
            # Generate two lists of header files in this directory
            files_h = list_files(d, ".h")
            files_hpp = list_files(d, ".hpp")

            # Skip this directory if no header files
            if (len(files_h) <= 0) and (len(files_hpp) <= 0):
                continue

            # Write conditional include directives for each .h header file
            if (len(files_h) > 0):
                output.write("    // .h files found in path: " + d + "\n")
                output.write("    #if " + str(include_setting(d)) + "\n")
                for f in files_h:
                    output.write("    #include \"" + "../" + os.path.join(d, f) + "\"\n")
                output.write("    #endif\n")
                output.write("\n")
            
            # Write conditional include directives for each .hpp header file
            if (len(files_hpp) > 0):
                output.write("    // .hpp files found in path: " + d + "\n")
                output.write("    #if " + str(include_setting(d)) + "\n")
                for f in files_hpp:
                    output.write("    #include \"" + "../" + os.path.join(d, f) + "\"\n")
                output.write("    #endif\n")
                output.write("\n")
        
        # Write include guards, at the end of file
        output.write("\n")
        output.write("#endif\n")

    # Program successfully finished

# Entry Point
if __name__ == "__main__":
    main()