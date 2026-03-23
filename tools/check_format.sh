#!/bin/bash
set -euo pipefail
find "$BUILD_WORKSPACE_DIRECTORY/mutatio" \( -iname "*.h" -o -iname "*.cc" \) \
    | xargs clang-format --dry-run --Werror
echo "clang-format check passed."
