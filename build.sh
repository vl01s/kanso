#!/usr/bin/env bash
# Build script for kanso engine

# Uncomment for debug
#set -x

# Print args to STDERR
error() {
    local TXT=("$@")
    printf "%s\n" "${TXT[@]}" >&2
    return 0
}

# Terminate script execution
die() {
    local EC=1

    if [[ $# -ge 1 ]] && [[ $1 =~ ^(0|-?[1-9][0-9]*)$ ]]; then
        EC=$1
        shift
    fi

    if [[ $# -ge 1 ]]; then
        local TXT=("$@")
        if [[ $EC -eq 0 ]]; then
            printf "%s\n" "${TXT[@]}"
        else
            error "${TXT[@]}"
        fi
    fi

    exit "$EC"
}

! [[ -d ./src ]] && exit 127

if [[ -z ${VULKAN_SDK+X} ]]; then
    die 127 "Vulkan environment not initialized. Aborting."
fi

# Determine if one or more commands exist in shell or not
_cmd() {
    [[ $# -eq 0 ]] && return 127

    local EC=0

    while [[ $# -gt 0 ]]; do
        if ! command -v "$1" &> /dev/null; then
            EC=1
            break
        fi

        shift
    done

    return "$EC"
}

if ! _cmd 'wayland-scanner' || [[ -f /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ]]; then
    wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
        ./src/xdg-shell-client-protocol.h

    wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
        ./src/xdg-shell-protocol.c

else
    echo -e "No xdg-shell code available!" >&2
fi

C_FILENAMES=$(find ./src -type f -regex '.*\.c$')
COMPILER_FLAGS=("-std=gnu17" "-g" "-Og" "-shared" "-fPIC")
# -Wall -Werror -pedantic
INCLUDE_FLAGS=("-I$VULKAN_SDK/include" "-Isrc")
LINKER_FLAGS=("-L$VULKAN_SDK/lib" "-lvulkan" "-lwayland-client" "-lm" "-shared")
OUT_DIR="bin"

mkdir -p ./"$OUT_DIR"

gcc $C_FILENAMES -o ./"$OUT_DIR"/libkansoengine.so \
    "${COMPILER_FLAGS[@]}" \
    "${INCLUDE_FLAGS[@]}" \
    "${LINKER_FLAGS[@]}"

# Make sure to kill debugging
#set +x
