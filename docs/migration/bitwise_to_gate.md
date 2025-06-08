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
