#!/bin/bash
OUTPUT=$1
g++ -static-libstdc++ -std=c++0x main.cpp watershell.cpp -o ./build/$OUTPUT
#g++ -m32 -static-libstdc++ -std=c++0x main.cpp watershell.cpp -o ../build/x86_$OUTPUT

