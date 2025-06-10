#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
COMP="$SRC_DIR/build/qppc"
IR="$DIR/out_hdr.ir"
"$COMP" "$SRC_DIR/docs/examples/teleport.qpp" "$IR"
grep -q '^#QUBITS' "$IR"
grep -q '^#GATES' "$IR"
grep -q '^#BYTES' "$IR"
grep -q '^CLIFFORD' "$IR"
rm "$IR"
