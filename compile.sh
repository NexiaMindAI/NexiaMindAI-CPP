#!/bin/bash
# compile.sh - Script to compile the Qt project

# Check if chatbot.pro exists, if not, create it.
if [ ! -f chatbot.pro ]; then
    echo "chatbot.pro file not found, creating one..."
    cat <<EOF > chatbot.pro
QT += widgets network
CONFIG += c++17
SOURCES += main.cpp
EOF
fi

echo "Generating Makefile using qmake..."
qmake chatbot.pro
if [ $? -ne 0 ]; then
    echo "qmake failed. Please ensure Qt development packages are installed."
    exit 1
fi

echo "Compiling project with make..."
make
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful. Run the application with: ./chatbot"

