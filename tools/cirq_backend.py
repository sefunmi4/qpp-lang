#!/usr/bin/env python3
"""Execute a minimal subset of QIR using Cirq."""
import sys
try:
    import cirq
except ImportError:
    print("Cirq not installed", file=sys.stderr)
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
        print('Usage: cirq_backend.py <qirfile>')
        return 1
    ops = parse_qir(sys.argv[1])
    max_q = max(op[1] if op[0] != 'ccx' else max(op[1], op[2], op[3]) for op in ops)
    qubits = [cirq.LineQubit(i) for i in range(max_q + 1)]
    circuit = cirq.Circuit()
    for op in ops:
        if op[0] == 'h':
            circuit.append(cirq.H(qubits[op[1]]))
        elif op[0] == 'x':
            circuit.append(cirq.X(qubits[op[1]]))
        elif op[0] == 'y':
            circuit.append(cirq.Y(qubits[op[1]]))
        elif op[0] == 'z':
            circuit.append(cirq.Z(qubits[op[1]]))
        elif op[0] == 's':
            circuit.append(cirq.S(qubits[op[1]]))
        elif op[0] == 't':
            circuit.append(cirq.T(qubits[op[1]]))
        elif op[0] == 'cx':
            circuit.append(cirq.CNOT(qubits[op[1]], qubits[op[2]]))
        elif op[0] == 'cz':
            circuit.append(cirq.CZ(qubits[op[1]], qubits[op[2]]))
        elif op[0] == 'ccx':
            circuit.append(cirq.CCX(qubits[op[1]], qubits[op[2]], qubits[op[3]]))
        elif op[0] == 'measure':
            circuit.append(cirq.measure(qubits[op[1]], key=str(op[1])))
    sim = cirq.Simulator()
    result = sim.run(circuit, repetitions=1)
    print(result)

if __name__ == '__main__':
    main()
