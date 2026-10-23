#!/usr/bin/env sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$SCRIPT_DIR"

usage() {
    echo "用法：$0 [debug|release|clean]"
    echo "  默认构建 release。"
}

case "${1:-release}" in
    clean)
        rm -rf build
        echo "已清理 build 目录。"
        ;;
    debug)
        cmake --preset ninja-debug
        cmake --build --preset ninja-debug
        echo "构建完成：build/ninja-debug/bin/gl-draw-desktop"
        ;;
    release)
        cmake --preset ninja-release
        cmake --build --preset ninja-release
        echo "构建完成：build/ninja-release/bin/gl-draw-desktop"
        ;;
    *)
        usage >&2
        exit 2
        ;;
esac
