#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
COMPILER="$SRC_DIR/build/qppc"
for f in "$SRC_DIR"/docs/examples/*.qpp; do
    echo "Compiling $f"
    out=$("$COMPILER" "$f" /tmp/out.ir 2>&1)
    echo "$out"
    echo "$out" | grep -q "Unrecognized syntax" && exit 1
    rm -f /tmp/out.ir
done
