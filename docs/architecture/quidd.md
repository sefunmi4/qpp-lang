# QuIDD-Based Compression

This document describes the simplified Quantum Information Decision Diagram (QuIDD) implementation used in the runtime.

The `QuIDD` class represents wavefunctions as shared decision nodes. Terminal nodes store complex amplitudes while internal nodes index qubits and reference their low/high children. Identical subtrees are merged to reduce memory usage.

```text
Node {
    Node* low;
    Node* high;
    std::size_t var;      // qubit index
    bool is_terminal;
    std::complex<double> value; // for terminal nodes
}
```

A wavefunction vector is recursively split in half per qubit while building the diagram. Equal subtrees collapse to a single node.

Conversion back to a dense vector simply expands the diagram over the full index range.

The executable `quidd_benchmark` generates random states and reports the memory savings compared to the dense wavefunction array.
