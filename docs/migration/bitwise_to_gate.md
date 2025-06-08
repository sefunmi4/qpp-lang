# Bitwise Operators and Quantum Gates

Classical bitwise operators translate to specific quantum gate sequences when the operands are backed by `qregister` memory. This table summarises the defaults used by the Q++ compiler:

| Operator | Quantum Equivalent |
|----------|-------------------|
| `^`      | Controlled-NOT (CX) |
| `&`      | Toffoli (CCX) |
| `|`      | Multi-controlled X |
| `~`      | X gate (bit flip) |

For example:

```cpp
qregister bool a, b;
bool c = a ^ b; // expands to CX with `a` as control and `b` as target
// compound assignment also maps to the same gate
a ^= b;       // emits CX with `a` as target
```

When operands are classical (`cregister` or regular `bool`) the compiler emits normal bitwise instructions.

Gate expansion applies only when every operand can be resolved to a qubit at compile time. Mixed expressions with classical and quantum values split into classical operations and quantum gates. For instance:

```cpp
cregister bool c[1];
qregister bool q[1];
c[0] ^= q[0]; // performs CX on q[0] then XOR into c[0]
```

Multi-qubit operators like `|` translate into a series of controlled-X gates with all but the last operand as controls. The compiler ensures the target register has enough qubits to address each index.

## Compound Assignments

Operators `|=`, `&=`, and `^=` mutate the target qubit or classical bit in place. When acting on `qregister` operands the assignment is converted to the corresponding controlled gate with the left-hand side as target.

```cpp
qregister bool a, b;
a ^= b;  // same as CX(b, a)
a &= b;  // expands to CCX(a, b, a)
```

`~` toggles a single qubit with an X gate and therefore has no binary operand.

## Mixing with Measurement

Because bitwise operations on qubits are unitary, results are not materialised until a measurement occurs:

```cpp
qregister bool x[1], y[1];
bool m = measure(x[0] ^ y[0]); // performs CX then measurement
```

If the measurement is deferred, the entanglement persists through later operations.

## Sample QIR Output

Compiling

```cpp
qregister bool a[1], b[1];
a[0] ^= b[0];
```

yields QIR similar to:

```
  call void @__quantum__qis__cnot(i64 0, i64 1)
```

where `0` and `1` refer to the hardware register mapping.
