# Q++ Frontend Specification

## 🚀 Vision
Q++ enables programmers to write code that "just runs" on classical or quantum hardware, with minimal syntactic overhead. By preserving the familiar syntax of C/C++, and augmenting it with new abstractions like `task<>`, `qalloc`, and probabilistic conditionals, Q++ bridges the gap between traditional system programming and quantum-aware development.

---

## 🧠 Design Philosophy
- **Everything is a Bit:** All bits are probabilistic unless stated otherwise (e.g., `cbits`).
- **Extend, Don’t Replace:** Use existing C/C++ conventions for bit structure and extend them with quantum logic when needed.
- **Dual-use by Default:** Prefer `auto` and unified `register` declarations. Developers can specify `qregister` or `cregister` if needed, similar to `task<QPU>` or `task<CPU>`.
- **Probabilistic Logic:** Boolean logic tied to `qbits` becomes probabilistic. Bitwise operations map to quantum gate macros (e.g., `&` → Toffoli, `^` → CX) where appropriate.
- **Structure Inference:** Allow structs (`qstruct`, `cstruct`) and classes (`qclass`, `cclass`) to model hybrid behavior, with types resolved at compile time based on field types.
- **Task Types:** Use `task<>` to explicitly define target hardware (e.g., `task<QPU>`, `task<CPU>`, `task<AUTO>`).
- **Inline Assembly + Macros:** Macros behave differently under the hood depending on classical or quantum context. Dual behavior is a feature, not a guarantee.
- **LLVM/QIR:** Use LLVM IR with optional QIR extensions for collapse tracking and probabilistic metadata.

---

## ✨ Frontend Language Features

### `task<>` Declarations [Issue #101](https://github.com/qpp-lang/qpp/issues/101)
```cpp
task<AUTO> hybrid_logic() {}
task<QPU> quantum_logic() {}
task<CPU> classical_logic() {}
```
- Defines hardware execution target
- Compiler infers best match under `AUTO`

### Conditional Expressions [Issue #102](https://github.com/qpp-lang/qpp/issues/102)
```cpp
if (q[0]) {
    // Marks surrounding scope as probabilistic
}
```
- Propagates probabilistic behavior to enclosing task
- Impacts scheduling and resolution at runtime

### Memory Allocation [Issue #103](https://github.com/qpp-lang/qpp/issues/103)
```cpp
qalloc int qarray[4];         // Quantum-backed array
qregister int reg1;           // Quantum bit register
register int r2;              // Inferred register
```
- `register` can default to CPU or QPU context
- Use `qregister` or `cregister` for explicit targeting

### Quantum-Aware Types [Issue #104](https://github.com/qpp-lang/qpp/issues/104)
```cpp
qstruct Token {
    qbit state;
    int index;
};
```
- Any structure with quantum members inherits probabilistic behavior

### Boolean and Bitwise Behavior [Issue #105](https://github.com/qpp-lang/qpp/issues/105)
```cpp
bool flag = q[0];             // Probabilistic boolean
int x = a & b;                // May map to Toffoli gate if quantum
```
- Boolean logic collapses under measurement
- Bitwise operators compile to classical or quantum gate macros based on scope

### Inline Assembly [Issue #106](https://github.com/qpp-lang/qpp/issues/106)
```cpp
asm volatile("movl $1, %eax");
```
- Treated as AUTO unless specifically QPU-restricted
- Compiler guards dual behavior with warnings

---

## 🛠 Compilation and Debugging Hints

### Collapse Markers [Issue #107](https://github.com/qpp-lang/qpp/issues/107)
- Probabilistic expressions trigger metadata emission in IR
- Useful for simulation and QPU hardware binding

### Measurement Behavior [Issue #108](https://github.com/qpp-lang/qpp/issues/108)
```cpp
measure(q[1]); // collapses q[1] and upstream scope
```
- Measurement finalizes quantum state

### Scope Mapping [Issue #109](https://github.com/qpp-lang/qpp/issues/109)
- Compiler emits visual map of probabilistic flow for debugging

---

## 🔁 Recap of Frontend Priorities
- Developer-first ergonomics
- Minimize mental overhead when porting C++ code
- Clearly communicate probabilistic behavior in familiar syntax
- Prepare code for hybrid execution with a single source of truth

---

*End of Frontend Spec v0.2*

