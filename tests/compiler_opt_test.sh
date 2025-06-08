#!/bin/sh
set -e
DIR="$(dirname "$0")"
SRC_DIR="$(cd "$DIR"/.. && pwd)"
IR="$DIR/out.ir"
"$SRC_DIR/build/qppc" "$DIR/optimize_sample.qpp" "$IR"
grep -q '^Z ' "$IR"
grep -q '^S ' "$IR"
if grep -q '^H ' "$IR"; then
  echo "H gate not optimized" >&2
  exit 1
fi
if grep -q '^X ' "$IR"; then
  echo "X gate not optimized" >&2
  exit 1
fi
rm "$IR"
