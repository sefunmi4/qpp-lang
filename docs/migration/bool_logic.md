# Probabilistic Boolean Semantics

Booleans derived from quantum memory behave probabilistically. Reading a `qbit` into a `bool` triggers a measurement, collapsing the qubit state. Until that point the value is undefined and must be treated as a superposition of `true` and `false`.

Conditional statements using such values are deferred to runtime:

```cpp
qregister bool qflag;
if (qflag) { /* executed only after measurement */ }
```

The runtime tracks these deferred branches and resolves them when a measurement occurs. Classical booleans (`cregister` or default `bool`) are evaluated normally.
