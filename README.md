# Q++ (QPP)

**Q++** is a new programming language designed for the hybrid era of quantum and classical computing. Inspired by C++ and built for the future, Q++ provides native syntax and runtime support for quantum-classical task orchestration, gesture-driven command execution, and spatial computing.

> 🧠 Built in C++. Powered by LLVM. Optimized for QPU + CPU.

---

## 🚀 Vision

Q++ (pronounced "Q plus plus") is for developers who want to:

- Write quantum and classical logic side by side
- Target real QPUs, simulated QPUs, or fallback CPUs
- Develop VR-native OS systems, like [EtherOS](https://github.com/shefashiru/qpp-etheros)
- Map gestures and symbols to code logic with [SymbolCast](https://github.com/shefashiru/qpp-symbolcast)

---

## 📦 Project Structure

This repo contains the **core Q++ compiler**, including:

- `src/` – Lexer, parser, AST builder, LLVM IR generator
- `include/` – Q++ headers and macros
- `tests/` – Q++ syntax and IR tests
- `examples/` – Sample `.qpp` programs
- `docs/` – Language spec, grammar, and IR model

---

## 💻 Example Code

```qpp
qtask<QPU> teleport(qreg<2> q) {
    h(q[0]);
    cx(q[0], q[1]);
    measure(q);
}
```
