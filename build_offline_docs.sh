#!/bin/env bash

# Basic shell script to build offline documents for OpenSD

SRC_DIR="./doc/"
DST_DIR="./build/"
ASCIIDOCTOR_BIN=asciidoctor
GZIP_BIN=gzip

# Make sure Asciidoctor is installed
if ! command -v "${ASCIIDOCTOR_BIN}" &> /dev/null; then
    printf "%s\n" "Error: Cannot find asciidoctor binary!"
    exit 1
fi

if ! command -v "${GZIP_BIN}" &> /dev/null; then
    printf "%s\n" "Error: Cannot find gzip binary!"
    exit 2
fi

# Create build directory
if [ ! -d "${DST_DIR}" ]; then
    if ! mkdir -p "${DST_DIR}"; then
        printf "%s\n" "Error:  Unable to create build directory '${DST_DIR}'"
        exit 3
    fi
fi

# Build user's manual
printf "%s\n" "Building HTML user manual..."
if ! "${ASCIIDOCTOR_BIN}" -R "${SRC_DIR}" -D "${DST_DIR}" -o users_manual.html "${SRC_DIR}"/_users_manual.adoc; then
    printf "%s\n" "Error: Failed to build user_manual.html"
    exit 4
fi

# Build opensdd(1) manpage
printf "%s\n" "Building opensdd manpage..."
if ! "${ASCIIDOCTOR_BIN}" -R "${SRC_DIR}" -D "${DST_DIR}" -b manpage "${SRC_DIR}"/_opensdd.1.adoc; then
    printf "%s\n" "Error: Failed to build manpage"
    exit 5
fi

# Compress it
if ! gzip -f "${DST_DIR}"opensdd.1; then
    printf "%s\n" "Error: Failed to compress opensdd.1"
    exit 6
fi
