# Q++: A Hybrid Classical-Quantum Programming Language

## Abstract

Q++ is an experimental programming language that extends the familiar syntax of C++ to the quantum domain. It provides a minimal set of new keywords and runtime components so that developers can write hybrid quantum-classical programs without abandoning systems-level paradigms. This white paper introduces the motivations behind Q++, outlines the key design principles, and summarizes the current implementation state alongside future research directions.

## 1. Introduction

Quantum computing promises exponential advantages for certain algorithms, but existing frameworks often require developers to adopt entirely new programming models. Q++ aims to reduce this barrier by preserving the structure of C++ while introducing quantum logic primitives. The language and accompanying runtime explore how classical and quantum tasks can coexist in a single program, enabling a smooth migration path for traditional developers.

## 2. Design Philosophy

Q++ builds upon three guiding principles:

1. **Minimal Syntax Overhead** – The language introduces a small set of new keywords, such as `qstruct`, `qalloc`, and `task<QPU>`, that extend C++ semantics instead of replacing them. Probabilistic branching uses familiar constructs like `if (q[0])` to condition on quantum states.
2. **Probabilistic Logic** – All bits are considered quantum by default. Classical bits (`cbits`) are specified explicitly so that code can reason about superposition and entanglement while still supporting deterministic logic when needed.
3. **Hardware-Hinted Tasks** – Developers can annotate functions with `task<CPU>`, `task<QPU>`, or `task<AUTO>` to hint how work should be scheduled. The runtime dispatches tasks to simulators or hardware accordingly.

## 3. Architecture Overview

The Q++ ecosystem consists of a prototype compiler (`qppc`) and a runtime executor (`qpp-run`). Together they parse source code, generate an intermediate representation, and simulate wavefunction evolution.

### 3.1 Frontend

The frontend combines regex-based tokenization with a small PEG parser. It translates `.qpp` source files into a typed intermediate representation (IR) that records gate applications, measurements, and task annotations. Future versions will introduce optimization passes and richer IR metadata.

### 3.2 Runtime

The runtime simulates quantum state evolution using a dense complex amplitude array. Planned features include sparse wavefunction storage, decision diagram (QuIDD) compression, and GPU acceleration. Scheduler logic interprets `task<>` hints and controls measurement collapse, random number generation, and memory tracking.

### 3.3 Hardware Integration

While current hardware backends are stubbed, Q++ is designed to interface with existing frameworks like Qiskit, Cirq, Q#, and Braket. Emitting QIR or OpenQASM will allow compiled programs to target real QPUs in the future.

## 4. Achievements

- **Prototype Compiler** – Parses basic syntax, generates IR, and supports inline assembly via `__qasm` blocks.
- **Wavefunction Simulator** – Executes simple quantum programs and provides deterministic random number generation for testing.
- **Scheduler Support** – Recognizes annotations such as `@dense`, `@clifford`, and `@hybrid` for runtime optimization.
- **Documentation Framework** – Organized docs describe migration strategies, architecture plans, and example programs.

## 5. Current Limitations

- The parser handles only a subset of C++ syntax; many constructs remain unsupported.
- Backend APIs are incomplete, so no real hardware integration is available yet.
- Documentation on some language features and optimizer passes is still in progress.
- Classical control flows interacting with quantum states remain experimental.

## 6. Roadmap

The project roadmap includes:

1. Implementing sparse and QuIDD-based wavefunction representations.
2. Completing backend emitters for QIR and OpenQASM.
3. Expanding optimization passes and formalizing the IR.
4. Developing a standard library for common quantum and classical utilities.
5. Migrating more C++ idioms to Q++ with thorough documentation and examples.

## 7. Conclusion

Q++ demonstrates how quantum and classical logic can coexist within a familiar systems programming style. By minimizing syntax changes and providing a modular runtime, Q++ lowers the barrier to experimenting with quantum algorithms. Ongoing work will focus on improved simulation performance, robust compiler infrastructure, and real hardware connections.

## Repository Overview

- `qppc/` – Compiler sources and parsing logic.
- `runtime/` – Wavefunction simulator, scheduler, and logging utilities.
- `docs/` – Design notes, migration guides, and example programs.
- `tests/` – Unit tests for the runtime and wavefunction components.

## References

1. IBM Qiskit Documentation
2. Google Cirq Documentation
3. LLVM QIR Specification
4. Decision Diagram Compression in Quantum Simulation (QuIDDs)

