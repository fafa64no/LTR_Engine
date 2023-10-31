#!/bin/bash
timestamp=$(date +%s)

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-unused-value"
includes="-Ithird_party -Ithird_party/Include -Isrc -Iincludes"

clang++ $includes -g src/main.cpp -oLTR_Engine.exe $libs $warnings