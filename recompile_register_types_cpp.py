# Author: Chun Ho Chan (Edward)
# Last Modified: 2024-09-28

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

# Main method. 
if __name__ == "__main__":
    # List of files that should not be included/ registered. 
    BLACKLIST = [
            "register_types.h", 
            "register_types.cpp", 
            "doc_data.gen.cpp", 
            "boilerplate_macros.h", 
            "globals.h"
        ]
    
    # Special token to indicate where to generating code. 
    # Tokens must be in a single-line comment, starting with "// " at the corrent indentation. 
    TOKEN = ">_<" # i.e. the face you make when you have to write boilerplate code. 

    # List of valid commands. 
    COMMANDS = [
            "headers_here", 
            "classes_here"
        ]

    # Generate a list of all directories. 
    subdirectories = list_subdirectories(".\\src")

    headers = []
    classes = []

    # Search each directory. 
    for d in subdirectories:
        # Generate lists of header/ source files in this directory. 
        temp_h = [f for f in list_files(d, ".h") if f not in BLACKLIST]
        temp_hpp = [f for f in list_files(d, ".hpp") if f not in BLACKLIST]
        temp_cpp = [f for f in list_files(d, ".cpp") if f not in BLACKLIST]

        # Store header names. 
        headers += temp_h
        headers += temp_hpp

        # Store class names. 
        for f in temp_cpp:
            with open(os.path.join(d, f), mode="r", encoding="utf-8") as source:
                # Extract the class name from the namespace of the static void _bind_methods() function, since it must be defined. 
                for line in source:
                    end = line.find("::_bind_methods()")
                    start = line.find("void")
                    if (end != -1) and (start != -1):
                        classes.append(line[start + 4 : end].strip())
                        break

    # Read/ Copy from the format file. Write to the header file. 
    with open(".\\register_types_format.cpp", mode="r", encoding="utf-8") as formatter, open(".\\src\\register_types.cpp", mode="w", encoding="utf-8") as output:
        for line in formatter:
            # Find the first token. 
            first = line.find(TOKEN)
            if first == -1:
                # First token is not found. 
                output.write(line)
                continue

            # Find the second token. 
            second = line.find(TOKEN, first + len(TOKEN))
            if second == -1:
                # Second token is not found. 
                output.write(line)
                continue

            # Find the third token, which shouldn't exist. 
            third = line.find(TOKEN, second + len(TOKEN))
            if third != -1:
                # Third token is found. 
                output.write(line)
                continue

            # Find and validate the text surrounded by the tokens. 
            text = line[first + len(TOKEN) : second].strip()
            if not text in COMMANDS:
                # Command is invalid. 
                output.write(line)
                continue

            # Find the text outside of the tokens
            prefix = line[0 : first].replace("// ", "", 1)
            suffix = line[second + len(TOKEN) : ]

            # Execute the command in this line. 
            if text == COMMANDS[0]:
                for t in headers:
                    output.write(prefix + t + suffix)
            elif text == COMMANDS[1]:
                for t in classes:
                    output.write(prefix + t + suffix)

    # Program successfully finished. 
    print("Done!\n")