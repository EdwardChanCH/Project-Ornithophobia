#!/usr/bin/env python
import os

env = SConscript("./godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

def list_subdirectories(directory):
    """
    Return a list of sub-directories, including the input directory. 
    """
    return [os.path.relpath(root).replace("\\", "/") + "/" for (root, dirs, files) in os.walk(directory)]

# Tweaked this to allow storing source code in sub-directories; original code (non-recursive):
#env.Append(CPPPATH=["src/"])
#sources = Glob("src/*.cpp")

subdirectories = list_subdirectories(".\\src")
env.Append(CPPPATH=subdirectories)
sources = [Glob(d + "*.cpp") for d in subdirectories]

# This code creates linking errors, for unknown reasons. 
#if env["target"] in ["editor", "template_debug"]:
#    doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
#    sources.append(doc_data)

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "game/bin/libcs3307.{}.{}.framework/libcs3307.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "game/bin/libcs3307.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "game/bin/libcs3307.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "game/bin/libcs3307{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
