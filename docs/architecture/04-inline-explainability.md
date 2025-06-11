# Inline Explainability

Q++ now supports an optional `#explain` directive for annotating quantum code blocks. When the compiler encounters `#explain` before a statement it emits a descriptive comment into the generated IR. The runtime prints these explanations during execution so developers can trace quantum state transitions.

```
#explain Preparing superposition on q[0]
H(q[0]);
```

If no custom message is provided the compiler supplies a basic explanation for common constructs such as gates, measurements and conditional branches. This works with the simulator and all hardware backends because `EXPLAIN` instructions are ignored by backends.

Use the directive freely when teaching or debugging to surface details like probabilistic branching, entanglement and measurement collapse.
