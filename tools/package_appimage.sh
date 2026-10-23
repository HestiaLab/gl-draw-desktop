#!/usr/bin/env bash
set -euo pipefail

VERSION=""
PLATFORM="linux-x64"
DIST_DIR="dist"
STAGE_DIR=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --version)
            VERSION="${2:-}"
            shift 2
            ;;
        --platform)
            PLATFORM="${2:-}"
            shift 2
            ;;
        --dist-dir)
            DIST_DIR="${2:-}"
            shift 2
            ;;
        --stage-dir)
            STAGE_DIR="${2:-}"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1" >&2
            exit 2
            ;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

if [[ -z "$VERSION" ]]; then
    echo "--version is required." >&2
    exit 1
fi

DIST_DIR="$(mkdir -p "$DIST_DIR" && cd "$DIST_DIR" && pwd)"
PACKAGE_NAME="gl-draw-desktop-v$VERSION-$PLATFORM"

if [[ -z "$STAGE_DIR" ]]; then
    STAGE_DIR="$DIST_DIR/stage/$PACKAGE_NAME"
fi

if [[ ! -x "$STAGE_DIR/bin/gl-draw-desktop" ]]; then
    echo "Missing staged gl-draw-desktop executable: $STAGE_DIR/bin/gl-draw-desktop" >&2
    exit 1
fi

APPDIR="$DIST_DIR/appimage/$PACKAGE_NAME.AppDir"
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin" "$APPDIR/usr/share/gldrawdesktop" "$APPDIR/usr/share/applications" "$APPDIR/usr/share/icons/hicolor/scalable/apps"

cp "$STAGE_DIR/bin/gl-draw-desktop" "$APPDIR/usr/bin/gl-draw-desktop"
if [[ -d "$STAGE_DIR/share/gldrawdesktop" ]]; then
    cp -R "$STAGE_DIR/share/gldrawdesktop/." "$APPDIR/usr/share/gldrawdesktop/"
fi
for dir in shaders themes scripts; do
    if [[ -d "$STAGE_DIR/bin/$dir" ]]; then
        cp -R "$STAGE_DIR/bin/$dir" "$APPDIR/usr/bin/"
    fi
done
if [[ -d "$STAGE_DIR/share/gldrawdesktop/fonts" ]]; then
    cp -R "$STAGE_DIR/share/gldrawdesktop/fonts" "$APPDIR/usr/bin/fonts"
fi

cp "packaging/linux/gl-draw-desktop.desktop" "$APPDIR/usr/share/applications/gl-draw-desktop.desktop"
cp "packaging/linux/gl-draw-desktop.svg" "$APPDIR/usr/share/icons/hicolor/scalable/apps/gl-draw-desktop.svg"
cp "packaging/linux/gl-draw-desktop.desktop" "$APPDIR/gl-draw-desktop.desktop"
cp "packaging/linux/gl-draw-desktop.svg" "$APPDIR/gl-draw-desktop.svg"

LINUXDEPLOY="$DIST_DIR/linuxdeploy-x86_64.AppImage"
if [[ ! -x "$LINUXDEPLOY" ]]; then
    curl -L -o "$LINUXDEPLOY" "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
    chmod +x "$LINUXDEPLOY"
fi

APPIMAGE_EXTRACT_AND_RUN=1 "$LINUXDEPLOY" --appdir "$APPDIR" --output appimage

# linuxdeploy 按 desktop 文件的 Name 生成文件名（GL_Draw_Desktop-x86_64.AppImage），
# 与可执行文件名不一定一致，因此按生成物定位而不是硬编码名称。
GENERATED_APPIMAGE=$(find . -maxdepth 1 -type f -name '*-x86_64.AppImage' -print -quit)
if [[ -z "$GENERATED_APPIMAGE" ]]; then
    echo "linuxdeploy did not produce an AppImage." >&2
    exit 1
fi
mv "$GENERATED_APPIMAGE" "$DIST_DIR/$PACKAGE_NAME.AppImage"

echo ""
echo "AppImage created:"
echo "  $DIST_DIR/$PACKAGE_NAME.AppImage"
