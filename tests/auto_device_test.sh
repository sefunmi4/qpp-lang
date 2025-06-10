#!/bin/sh
set -e
DIR="$(dirname "$0")"
RUN="$(cd "$DIR"/.. && pwd)/build/qpp-run"
IR="$DIR/auto_device.ir"
cat >"$IR" <<EOT
TASK auto CPU
ENGINE STABILIZER
#QUBITS 3
#GATES 0
#BYTES 67108864
CLIFFORD 1
TASK auto CPU
QALLOC q 1
ENDTASK
EOT
"$RUN" --auto-device "$IR" >out.txt
grep -q 'Estimated qubits:' out.txt
rm -f "$IR" out.txt
