#!/bin/bash

libs=-luser32
warnings=-Wno-writable-strings

clang src/main.cpp -LTR_Engine.exe $libs $warnings