# Q++ Functions and Task Declarations 

## 🔍 Overview
In Q++, functions are first-class citizens, just like in C++, but are extended to support quantum-classical hybrid execution using `task<>`. The `task<>` wrapper determines the execution context (CPU, QPU, AUTO) and is the central mechanism for building scoped, predictable hybrid programs.

This document outlines how functions are declared, dispatched, resolved, and scoped in Q++, and the rules for inheritance, overloading, and interaction with quantum state.

---

## 🧠 Function Types and Syntax

### Traditional Functions
```cpp
int add(int a, int b) {
    return a + b;
}
```
Behaves like standard C++ functions. If no quantum logic is used, functions run on CPU.

---

### Hybrid Task Functions 
```cpp
task<AUTO> my_algorithm() {
    if (q[0]) {
        // probabilistic block
    }
}
```
- `AUTO` will infer from inner logic and data
- `QPU` or `CPU` can be explicitly specified

#### Supported Task Modes:
- `task<CPU>` — guaranteed classical logic only
- `task<QPU>` — guaranteed quantum logic execution only
- `task<AUTO>` — inferred by scope and contents
- `task<MIXED>` — explicitly allows CPU and QPU operations in one task

---

## 📌 Scoping Rules

### Probabilistic Inference 
If any conditional or operation relies on a quantum bit:
- The function is tagged as probabilistic
- All enclosing scopes and parent functions inherit this tag
- Can only be resolved via collapse or deferred execution

---

## 🔁 Function Composition

### Calling Across Task Types 
Calling a `task<QPU>` from a `task<CPU>` requires:
- An explicit dispatch directive or
- Runtime handoff API

Calling `task<CPU>` from within `task<QPU>` is allowed but may trigger scope collapse if classical data is read.

---

## 🛠 Overloading and Templates

### Function Overloading 
Overloads must declare matching task signatures:
```cpp
task<CPU> compute(int a);
task<QPU> compute(qbit q);
```

### Templated Task Functions 
```cpp
template<typename T>
task<AUTO> compute(T input);
```
Compiler resolves hardware context based on `T`.

---

## ⚠️ Restrictions and Errors

### Invalid Mixing
```cpp
int classical_fn() {
    if (q[0]) return 1; // ❌ illegal unless tagged as task<>
}
```
- All functions using `qbits` must be marked with `task<>`

### Illegal Recursion
Recursion through unresolved probabilistic scopes is disallowed unless:
- Quantum state is measured inside
- Memoization occurs

---

## 📋 Notes for MVP
- All `task<>` functions return synchronously unless deferred
- Asynchronous quantum dispatch to be explored later
- Overloads must respect hardware contexts

---

*End of Function & Task Spec v0.1*

