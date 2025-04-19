# Q++: The Quantum-Classical Successor to C++

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

> Death to coders' pride:
> It's C++ but with some patches under the hood to describe quantum bits and their logic where necessary.
> This provides a shared perspective for code on computers with CPUs, GPUs, TPUs, and QPUs attached to them.
>
> Q++ assumes all bits are quantum (qbits) or classical (cbits) information unless specified.  
> It treats classical logic as a special case of general information.

---

## 🤝 Want to Contribute?

Open an issue or pick a task from `/docs/migration/`.

qpp-lang/
├── frontend/                  # Lexer, parser, grammar rules
│   ├── grammar/              # DSL: PEG/ANTLR/Bison rules
│   ├── lexer/                # Tokenizer
│   ├── parser/               # AST builder
│   └── translator/           # Q++ → C++ translator for bootstrapping
│
├── qpp/                      # Q++ headers + standard definitions
│   ├── core/                 # qalloc, qguard, task<>
│   ├── runtime/              # runtime hooks (scheduler, gates, collapse)
│   ├── macros/               # task_macros.h, shorthand templates
│   ├── types/                # bit abstractions, qbit/cbit structs
│   └── stdlib/               # math, gates, bitops, etc.
│
├── backend/                  # IR generation and target-specific logic
│   ├── irgen/                # LLVM IR / QIR emitter
│   ├── qsim/                 # Ideal simulator using your wavefunction math
│   └── codegen/              # Output C++/LLVM/QIR, or wasm later
│
├── runtime/                  # Runtime engine (external to stdlib)
│   ├── scheduler.cpp         # Handles task<> scheduling
│   ├── memory.cpp            # Handles qbit/cbit allocation
│   └── engine.cpp            # Wavefunction core / backend switch
│
├── migration/                # Migration + design docs
│   ├── 00-overview.md        # Your core design doc w/ links to issues
│   ├── 01-frontend.md        # Syntax support + grammar changes
│   ├── 02-runtime.md         # Quantum runtime layers
│   └── 03-hardware-api.md    # IBM/Google API integration plan
│
├── examples/                 # test programs and future demos
│   ├── teleport.qpp
│   ├── hello_world.qpp
│   └── adaptive_task.qpp
│
├── tests/                    # Unit tests and regression coverage
│   ├── regression/
│   └── unit/
│
├── tools/                    # CLI tools, helper scripts, migration utilities
│   ├── qppc.cpp              # Compiler entrypoint
│   └── qpp-run.cpp           # For simulation + execution
│
├── include/                  # Legacy compatibility, eventually deprecated
│   └── qpp_legacy.h
│
├── CMakeLists.txt
└── README.md

Built with ❤️ by @sefunmi4 and collaborators.
