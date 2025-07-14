#!/usr/bin/env bash
set -euo pipefail

# compile.sh - Script to compile the Qt project

# Variables
PRO_FILE="chatbot.pro"
BUILD_DIR="build"
TARGET_NAME="chatbot"

# Create and enter build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Run qmake and generate Makefile
echo "Running qmake..."
qmake "../${PRO_FILE}"

# Build the project
echo "Building..."
make -j"$(nproc)"

# Copy the binary back to the root directory (with .exe extension if possible)
if [ -f "${TARGET_NAME}" ]; then
  cp "${TARGET_NAME}" "../${TARGET_NAME}.exe" || cp "${TARGET_NAME}" "../${TARGET_NAME}"
fi

echo "Compilation successful. Binary available in the project root."
