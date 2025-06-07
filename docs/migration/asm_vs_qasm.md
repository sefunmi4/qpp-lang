# Inline Assembly vs Inline QASM

Q++ allows embedding low level code for both the CPU and the QPU.
Classical `asm` blocks behave exactly like they do in C++ and are executed on the CPU.
The `__qasm` keyword introduces a block of quantum assembly instructions that
operate on `qregister` state.

```cpp
asm volatile("movl $1, %eax");       // CPU assembly
__qasm {
  H 0;
  CX 0,1;
}
```

The compiler keeps these sections separate so that quantum instructions can be
sent to a simulator or real device while classical assembly executes normally.
