#!/bin/bash
set -euo pipefail

BUILD_DIR=$1

gcovr -j4 -s \
    --filter "mutatio/(src|include)/" \
    --exclude ".*/_deps/.*" \
    --html-details "${BUILD_DIR}/coverage/coverage.html"
