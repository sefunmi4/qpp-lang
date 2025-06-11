#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
COMPILER="$SRC_DIR/build/qppc"
for f in "$SRC_DIR/docs/examples/wavefunction_demo.qpp" "$SRC_DIR/docs/examples/teleport.qpp" "$SRC_DIR/docs/examples/adaptive_task.qpp" "$SRC_DIR/docs/examples/explain_demo.qpp"; do
    echo "Compiling $f"
    out=$("$COMPILER" "$f" /tmp/out.ir 2>&1)
    echo "$out"
    case "$f" in
        *bitwise_demo.qpp|*demo.qpp|*hello_world.qpp)
            : ;;
        *)
            echo "$out" | grep -q "Unrecognized syntax" && exit 1
            ;;
    esac
    rm -f /tmp/out.ir
done
