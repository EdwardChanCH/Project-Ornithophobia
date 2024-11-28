#!/usr/bin/env python
import os

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Tweak this if you want to use different folders, or more folders, to store your source code in.
# env.Append(CPPPATH=["src/"])
# sources = Glob("src/*.cpp")

# Recursive search for subfolders in "src".
subfolders = [os.path.relpath(root).replace("\\", "/") + "/" for (root, dirs, files) in os.walk("src")]
if "src/gen/" in subfolders:
    subfolders.remove("src/gen/") # exclude the folder of "doc_data.gen.cpp", or else SCons would crash

env.Append(CPPPATH=subfolders)
env.Append(CCFLAGS=["/EHsc"]) # Specify compiler's exception handling model

sources = []
for d in subfolders:
    sources.extend(Glob(d + "*.cpp"))

# Link custom doc files to the Godot Editor.
if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.extend(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# Library formats.
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
