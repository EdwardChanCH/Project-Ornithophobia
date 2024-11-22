"""
Description:
    This Python script generates 'custom_class.h' and 'custom_class.cpp' in './src/'.
Author:
    Chun Ho Chan (Edward)
Version:
    v1.0 (2024-11-10)
License:
    GNU Affero General Public License v3.0
"""

import os

def generate_file(class_name, file_extension):
    """
    Generates a file from template with the specified class name and file extension.
    """
    # Special token to indicate where to insert the class name
    TOKEN = "___"

    # List of valid commands
    COMMANDS = [
            "class_name_here", # Filename
            "CLASS_NAME_HERE", # Include Guard
            "ClassNameHere", # Class Name
            "Class Name Here" # Doxygen Comment; User Input
        ]
    
    # Generate all the name formats
    names = []
    names.insert(0, class_name.replace(" ", "_").lower())
    names.insert(1, class_name.replace(" ", "_").upper())
    names.insert(2, class_name.replace(" ", ""))
    names.insert(3, class_name)

    # Find the template file
    file_extension = file_extension.lower()
    if file_extension in [".h", ".cpp"]:
        FORMAT_FILE = "custom_class_format" + file_extension
        OUTPUT_FILE = ".\\src\\" + names[0] + file_extension
    else:
        return # Invalid file extension

    # Read/ Copy from the format file. Write to the header file
    with open(FORMAT_FILE, mode="r", encoding="utf-8") as formatter, open(OUTPUT_FILE, mode="w", encoding="utf-8") as output:
        for line in formatter:
            # Reset head to start of the line
            head = 0

            while True:
                # Find the head token
                head = line.find(TOKEN, head)
                if head == -1:
                    # Head token is not found
                    output.write(line)
                    break

                # Find the tail token. 
                tail = line.find(TOKEN, head + len(TOKEN))
                if tail == -1:
                    # Tail token is not found
                    output.write(line)
                    break

                # Find and validate the text surrounded by the tokens
                text = line[head + len(TOKEN) : tail]
                if not text in COMMANDS:
                    # Command is invalid
                    output.write(line)
                    break

                # Find the text outside of the tokens
                prefix = line[0 : head]
                suffix = line[tail + len(TOKEN) : ]

                # Execute the left-most command in this line
                line = prefix + names[COMMANDS.index(text)] + suffix

                # Update head to avoid scanning the replacement string
                head = head + len(names[COMMANDS.index(text)])

                # Check if no more token is following
                if line.find(TOKEN, head) == -1:
                    output.write(line)
                    break
            # repeat-until loop ends
        # for loop ends

def main():
    """
    Main method.
    """    
    # Get input
    input_name = input("Enter the class name (format: \"GD Custom Class 1\"): ")
    input_name.replace("\"", "").strip()

    # Generate both files
    generate_file(input_name, ".h")
    generate_file(input_name, ".cpp")
    
    # Program successfully finished

# Entry Point
if __name__ == "__main__":
    main()