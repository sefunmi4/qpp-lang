#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/demo.qpp" demo.ir
"$SRC_DIR/build/qpp-run" demo.ir
"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/bitwise_demo.qpp" bitwise.ir
"$SRC_DIR/build/qpp-run" bitwise.ir

"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/wavefunction_demo.qpp" wavefun.ir
"$SRC_DIR/build/qpp-run" wavefun.ir
"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/adaptive_task.qpp" adapt.ir
"$SRC_DIR/build/qpp-run" adapt.ir
"$SRC_DIR/build/qppc" "$SRC_DIR/docs/examples/teleport.qpp" teleport.ir
"$SRC_DIR/build/qpp-run" teleport.ir

