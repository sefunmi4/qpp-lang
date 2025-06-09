#!/usr/bin/env python3
"""Execute a minimal subset of QIR using Amazon Braket local simulator."""
import sys
try:
    from braket.circuits import Circuit
    from braket.devices import LocalSimulator
except Exception as e:
    print("Braket SDK not installed", file=sys.stderr)
    sys.exit(1)

def parse_qir(path):
    ops = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line.startswith('call void @__quantum__qis__H'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('h', idx))
            elif line.startswith('call void @__quantum__qis__X'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('x', idx))
            elif line.startswith('call void @__quantum__qis__Y'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('y', idx))
            elif line.startswith('call void @__quantum__qis__Z'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('z', idx))
            elif line.startswith('call void @__quantum__qis__S'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('s', idx))
            elif line.startswith('call void @__quantum__qis__T'):
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('t', idx))
            elif line.startswith('call void @__quantum__qis__cnot'):
                parts = line.split('i64')
                c = int(parts[1].split(',')[0])
                t = int(parts[2].split(')')[0])
                ops.append(('cx', c, t))
            elif line.startswith('call void @__quantum__qis__cz'):
                parts = line.split('i64')
                c = int(parts[1].split(',')[0])
                t = int(parts[2].split(')')[0])
                ops.append(('cz', c, t))
            elif line.startswith('call void @__quantum__qis__ccx'):
                parts = line.split('i64')
                c1 = int(parts[1].split(',')[0])
                c2 = int(parts[2].split(',')[0])
                t = int(parts[3].split(')')[0])
                ops.append(('ccx', c1, c2, t))
            elif '@__quantum__qis__measure' in line:
                idx = int(line.split('i64')[1].split(')')[0])
                ops.append(('measure', idx))
    return ops

def main():
    if len(sys.argv) < 2:
        print('Usage: braket_backend.py <qirfile>')
        return 1
    ops = parse_qir(sys.argv[1])
    max_q = max(op[1] if op[0] != 'ccx' else max(op[1], op[2], op[3]) for op in ops)
    circuit = Circuit()
    for op in ops:
        if op[0] == 'h':
            circuit.h(op[1])
        elif op[0] == 'x':
            circuit.x(op[1])
        elif op[0] == 'y':
            circuit.y(op[1])
        elif op[0] == 'z':
            circuit.z(op[1])
        elif op[0] == 's':
            circuit.s(op[1])
        elif op[0] == 't':
            circuit.t(op[1])
        elif op[0] == 'cx':
            circuit.cnot(op[1], op[2])
        elif op[0] == 'cz':
            circuit.cz(op[1], op[2])
        elif op[0] == 'ccx':
            circuit.ccnot(op[1], op[2], op[3])
        elif op[0] == 'measure':
            circuit.measure(op[1])
    device = LocalSimulator()
    task = device.run(circuit, shots=1)
    result = task.result()
    print(result.measurement_counts)

if __name__ == '__main__':
    main()
