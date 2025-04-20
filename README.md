# Q++

**Q++** is a modern programming language built to extend C++ into the quantum era.  
Inspired by how C++ evolved from C, Q++ introduces just enough new logic to support quantum-classical hybrid computing — while keeping everything great about systems-level development.

---

## 📌 Main Goals

- Build Q++ using C and C++ the way C++ was built from C
- Document how to migrate from C++ to Q++ with minimal cognitive overhead
- Let quantum logic flow naturally through familiar constructs like `struct`, `register`, `bool`, and `function`

---

## 🔧 What Q++ Adds

| Feature | Q++ Evolution |
|--------|----------------|
| `struct`, `class` | `qstruct`, `qclass`, `cstruct`, `cclass` for hybrid modeling |
| `bool` | Automatically probabilistic if linked to quantum memory |
| Bitwise Ops | Become quantum gate macros (`^` → `CX`, `&` → `Toffoli`) |
| `register` | Extended as `qregister`, `cregister`, or left as `auto` |
| `task<T>` | `task<QPU>`, `task<CPU>`, `task<AUTO>` target hardware like GPU kernels |
| `__qasm` | Inject raw gate-level code (like inline asm) |
| LLVM IR | Enhanced with QIR, collapse metadata, and probabilistic flags |

---

## 📘 Two Main Project Areas

### 📚 1. Developer Docs
Found in `/docs`. Help us:
- Translate C++ constructs into Q++ logic
- Define rules for probabilistic branching and hybrid task types
- Help classical devs think in quantum terms

### 🛠️ 2. Compiler Development
Found in `/src`. Help us:
- Parse new `qstruct`, `task<CPU>`, and `qregister` keywords
- Build IR with gate macros, entanglement tracking, and auto-collapse
- Simulate CPU/QPU behavior in a fallback runtime

---

## ✨ Philosophy

> No coders' pride just scribes:
> 
> It's C++ but with some patches under the hood to describe quantum bits and their logic where necessary.
> This provides a shared perspective for coding on computers with CPUs and QPUs attached to them.
>
> Q++ assumes all bits are quantum (qbits) or classical (cbits) information unless specified.  
> It treats classical logic as a special case of general information.

---

## 🤝 Want to Contribute?

Open an issue or pick a task from the [docs folder](https://github.com/sefunmi4/qpp-lang/blob/main/docs/overview.md#q-docs-overview).

Built with ❤️ by @sefunmi4 and collaborators.


