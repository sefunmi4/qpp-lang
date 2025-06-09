#!/usr/bin/env python3
"""Execute a minimal subset of QIR using PsiQuantum's Cirq-based simulator."""
import sys
try:
    import cirq
except Exception:
    print("PsiQuantum simulator not available", file=sys.stderr)
    sys.exit(1)

# Placeholder reusing cirq execution
from cirq import Simulator

# Reuse parser from cirq backend
from cirq_backend import parse_qir

def main():
    if len(sys.argv) < 2:
        print('Usage: psi_backend.py <qirfile>')
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
    sim = Simulator()
    result = sim.run(circuit, repetitions=1)
    print(result)

if __name__ == '__main__':
    main()
