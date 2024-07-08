#!/usr/bin/env bash
# Build script for kanso engine
mkdir -p ./bin
C_FILENAMES=$(find ./src -type f -name "*.c")
COMPILER_FLAGS="-std=gnu17 -g -Og -shared -fPIC"
# -Wall -Werror
INCLUDE_FLAGS="-Isrc -I$VULKAN_SDK/include"
LINKER_FLAGS="-lvulkan -lwayland-client -L$VULKAN_SDK/lib"

gcc $C_FILENAMES $COMPILER_FLAGS -o ./bin/libkansoengine.so $INCLUDE_FLAGS $LINKER_FLAGS
