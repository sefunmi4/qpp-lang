#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
COMP="$SRC_DIR/build/qppc"
RUN="$SRC_DIR/build/qpp-run"
"$COMP" "$SRC_DIR/docs/examples/teleport.qpp" tele.ir >/tmp/compile.log
grep -q "ENGINE" tele.ir
out=$("$RUN" tele.ir)
echo "$out" | grep -q "stabilizer"
rm -f tele.ir /tmp/compile.log
