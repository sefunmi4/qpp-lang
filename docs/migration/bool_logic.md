# Probabilistic Boolean Semantics

Booleans derived from quantum memory behave probabilistically. Reading a `qbit` into a `bool` triggers a measurement, collapsing the qubit state. Until that point the value is undefined and must be treated as a superposition of `true` and `false`.

Conditional statements using such values are deferred to runtime:

```cpp
qregister bool qflag;
if (qflag) { /* executed only after measurement */ }
```

The runtime tracks these deferred branches and resolves them when a measurement occurs. Classical booleans (`cregister` or default `bool`) are evaluated normally.

When a `qregister` element is coerced into a boolean, the compiler generates a runtime measurement instruction. Branching expressions therefore cause side effects:

```cpp
bool b = measure(q[0]);
if (b && other) {
    // `q[0]` collapsed above
}
```

In loops the measurement result is cached so subsequent evaluations of the same variable use the measured value. Developers should consider resetting qubits or re-preparing the state if multiple passes are needed.

## Short-Circuit Behavior

Logical operators such as `&&` and `||` still short-circuit. When the first operand involves a qubit, the measurement is performed immediately. The second operand is evaluated only if needed based on that outcome.

```cpp
qregister bool q0, q1;
if (measure(q0) && measure(q1)) {
    // q0 measured first; q1 measured only when q0 == true
}
```

## Side Effects in Loops

Qubits measured inside loops collapse on the first iteration.
To reuse the same quantum state across iterations you must reset or reallocate the qubits.

```cpp
for (int i = 0; i < 3; ++i) {
    int bit = measure(q[i]);
    // q[i] collapsed; prepare again if needed
}
```

## Boolean Return Values

Functions that return `bool` may hide measurements. Document these side effects clearly so callers understand the underlying qubits will be collapsed.

```cpp
bool check(qregister q[1]) {
    return measure(q[0]);
}
```
