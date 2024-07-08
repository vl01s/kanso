#!/bin/bash
mkdir -p ../bin

C_FILENAMES=$(find . -type f -name "*.c")

COMPILER_FLAGS="-g -Og -shared"
# -fdeclspec
# -fPIC
# -fms-extensions
# -Wall -Werror

INCLUDE_FLAGS="-Isrc"
# -I$VULKAN_SDK/include

LINKER_FLAGS="-lwayland-client -lm -lvulkan"

# DEFINES=""

# Generate xdg-shell client header file and glue code (wayland)
wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
    ./xdg-shell-client-protocol.h
wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
    ./xdg-shell-protocol.c

gcc $C_FILENAMES $COMPILER_FLAGS -o ../bin/libkansoengine.so $INCLUDE_FLAGS $LINKER_FLAGS # $DEFINES
