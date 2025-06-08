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

