#!/usr/bin/env bash

set -euf -o pipefail

# This line will only work in scripts and not sourced bash scripts.
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
cd "$SCRIPTPATH"

wc -l $(find src -name '*.c') $(find src -name '*.h') $(find src -name '*.y') $(find src -name '*.lex')
