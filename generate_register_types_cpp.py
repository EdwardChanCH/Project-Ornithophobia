"""
Description:
    This Python script generates 'register_types.cpp' in './src/'.
Author:
    Chun Ho Chan (Edward)
Version:
    v1.1 (2024-11-04)
License:
    GNU Affero General Public License v3.0
"""

import os
import shutil

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

def backup_file(original_file, backup_file):
    """
    Creates a backup file.
    """
    shutil.copyfile(os.path.abspath(original_file), os.path.abspath(backup_file))

def generate_file(format_file, output_file):
    """
    Update the output file based on template. Include headers and registers classes (with a _bind_methods() declaration in .cpp file).
    """
    # List of files that should not be included/ registered
    BLACKLIST = [
            "register_types.h", 
            "register_types.cpp", 
            "doc_data.gen.cpp", 
            "boilerplate_macros.h", 
            "globals.h"
        ]
    
    # Special token to indicate where to generating code
    # Tokens must be in a single-line comment, starting with "// " at the corrent indentation
    TOKEN = ">_<" # i.e. the face you make when you have to write boilerplate code

    # Special boundary to look for existing included headers and registered classes
    BOUNDARY = "/*--------------------------------------------------*/"

    # List of valid commands
    COMMANDS = [
            "headers_here", 
            "classes_here"
        ]

    # Generate a list of all directories
    subdirectories = list_subdirectories(".\\src")

    headers = []
    classes = []

    # Search each directory
    for d in subdirectories:
        # Generate lists of header/ source files in this directory
        temp_h = [f for f in list_files(d, ".h") if f not in BLACKLIST]
        temp_hpp = [f for f in list_files(d, ".hpp") if f not in BLACKLIST]
        temp_cpp = [f for f in list_files(d, ".cpp") if f not in BLACKLIST]

        # Store header names
        headers += temp_h
        headers += temp_hpp

        # Store class names
        for f in temp_cpp:
            with open(os.path.join(d, f), mode="r", encoding="utf-8") as source:
                # Extract the class name from the namespace of the static void _bind_methods() function, since it must be defined
                for line in source:
                    tail = line.find("::_bind_methods()")
                    head = line.find("void")
                    if (tail != -1) and (head != -1):
                        classes.append(line[head + 4 : tail].strip())
                        break

    # Scan the format file for existing included headers and registered classes
    with open(format_file, mode="r", encoding="utf-8") as formatter:
        within_boundary = False
        for line in formatter:
            # Check for boundary
            if (line.strip() == BOUNDARY):
                within_boundary = not within_boundary

            if within_boundary:
                # Skip over any comment
                pos = line.strip().find("//")
                if (pos == 0):
                    continue

                # Find existing included headers
                pos = line.find("#include")
                if (pos != -1):
                    # Extract header
                    head = line.find("<", pos + len("#include"))
                    if (head != -1):
                        tail = line.find(">", head + 1)
                    else:
                        head = line.find("\"", pos + len("#include"))
                        tail = line.find("\"", head + 1)

                    # Exclude this header from being added (again)
                    temp = line[head + 1 : tail].strip()
                    if temp in headers:
                        headers.remove(temp)
                    else:
                        print("\nWarning: File '" + temp + "' is not found!\n")
                    continue
                    
                # Find existing registered classes
                pos = line.find("GDREGISTER")
                if (pos != -1):
                    # Extract class
                    head = line.find("(", pos + len("GDREGISTER"))
                    tail = line.find(")", head + 1)
                    
                    # Exclude this class from being registered (again)
                    temp = line[head + 1 : tail].strip()
                    if temp in classes:
                        classes.remove(temp)
                    else:
                        print("\nWarning: File '" + temp + "' is not found!\n")
                    continue
    
    if (len(headers) == 0) and (len(classes) == 0):
        # Nothing to update
        print("No update needed.")
        return

    # Read/ Copy from the format file. Write to the header file
    with open(format_file, mode="r", encoding="utf-8") as formatter, open(output_file, mode="w", encoding="utf-8") as output:            
        for line in formatter:
            # Find the first token
            first = line.find(TOKEN)
            if first == -1:
                # First token is not found
                output.write(line)
                continue

            # Find the second token. 
            second = line.find(TOKEN, first + len(TOKEN))
            if second == -1:
                # Second token is not found
                output.write(line)
                continue

            # Find the third token, which shouldn't exist
            third = line.find(TOKEN, second + len(TOKEN))
            if third != -1:
                # Third token is found
                output.write(line)
                continue

            # Find and validate the text surrounded by the tokens
            text = line[first + len(TOKEN) : second].strip()
            if not text in COMMANDS:
                # Command is invalid
                output.write(line)
                continue

            # Find the text outside of the tokens
            prefix = line[0 : first].replace("// ", "", 1)
            suffix = line[second + len(TOKEN) : ]

            # Execute the command in this line
            if text == COMMANDS[0]:
                output.write(line)
                for t in headers:
                    output.write(prefix + t + suffix)
            elif text == COMMANDS[1]:
                output.write(line)
                for t in classes:
                    output.write(prefix + t + suffix)
    
    if len(headers) > 0:
        print("Included new headers:\n- " + "\n- ".join(headers) + "\n")
    if len(classes):
        print("Registered new classes:\n- " + "\n- ".join(classes) + "\n")

def main():
    FORMAT_FILE = ".\\register_types_format.cpp"
    OUTPUT_FILE = ".\\src\\register_types.cpp"
    BACKUP_FILE = ".\\_backup_register_types.cpp"
    
    if os.path.isfile(OUTPUT_FILE):
        print("Backing up '" + OUTPUT_FILE + "' to '" + BACKUP_FILE + "' ...")
        backup_file(OUTPUT_FILE, BACKUP_FILE)
        print("Updating " + OUTPUT_FILE + " ...")
        generate_file(BACKUP_FILE, OUTPUT_FILE)
    else:
        print("Creating '" + OUTPUT_FILE + "' using '" + FORMAT_FILE + "' ...")
        generate_file(FORMAT_FILE, OUTPUT_FILE)

    # Program successfully finished

# Entry Point
if __name__ == "__main__":
    main()