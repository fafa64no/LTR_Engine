#!/bin/bash
timestamp=$(date +%s)

libs="-luser32 -lopengl32 -lgdi32 -lthird_party/Lib/freetype.lib"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-unused-value"
includes="-Ithird_party -Ithird_party/Include -Ithird_party/freetype 
-Isrc -Isrc/game -Isrc/physics -Isrc/platform -Isrc/rendering
-Iincludes -Iincludes/game -Iincludes/physics -Iincludes/platform -Iincludes/rendering"

clang++ $includes -g src/main.cpp -oLTR_Engine.exe $libs $warnings