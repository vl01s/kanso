#!/usr/bin/env bash
# Build script for kanso engine

# Uncomment for debug
#set -x

C_FILENAMES=$(find ./src -type f -name "*.c")
COMPILER_FLAGS=("-std=gnu17" "-g" "-Og" "-shared" "-fPIC")
# -Wall -Werror -pedantic
INCLUDE_FLAGS=("-Isrc" "-I$VULKAN_SDK/include")
LINKER_FLAGS=("-lvulkan" "-lwayland-client" "-lm" "-L$VULKAN_SDK/lib" "-shared")
OUT_DIR="bin"

wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
    ./xdg-shell-client-protocol.h
wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
    ./xdg-shell-protocol.c
mkdir -p ./"$OUT_DIR"

gcc $C_FILENAMES -o ./"$OUT_DIR"/libkansoengine.so "${COMPILER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "${LINKER_FLAGS[@]}"

# Make sure to kill debugging
#set +x
