#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
COMP="$SRC_DIR/build/qppc"
PROFILE="$SRC_DIR/tests/small_profile.json"
set +e
"$COMP" "$SRC_DIR/docs/examples/teleport.qpp" out.ir --profile "$PROFILE" >out.txt 2>&1
status=$?
set -e
if [ "$status" -eq 0 ]; then
  echo "Compilation should have failed" >&2
  cat out.txt >&2
  rm -f out.ir out.txt
  exit 1
fi
grep -q "qubits" out.txt
rm -f out.ir out.txt
