#! /usr/bin/bash

_source_files=$(find src/ -type f -name "*.cpp")
echo $_source_files

_main_source="main.cpp"
if [ "$1" = "test" ]; then
    _main_source=$(find testing/ -type f -name "*.cpp")
fi

echo $_main_source

g++ -Wall -fsanitize=address -Iinclude $_source_files $_main_source -o outFile