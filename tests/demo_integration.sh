#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/demo.qpp" demo.ir
"$SRC_DIR/build/qpp-run" demo.ir
