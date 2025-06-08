# Q++ Docs Overview [#7](https://github.com/sefunmi4/qpp-lang/issues/7#issue-3006702643)

Welcome to the Q++ documentation system. This folder is organized to help both new and experienced developers contribute effectively, understand core concepts, and track the evolution of Q++ as a hybrid quantum-classical programming language.

---

## 🔍 Purpose of This Structure

This documentation is split into **three major sections**:

### 📐 `./architecture/` – Implementation Plans [#11](https://github.com/sefunmi4/qpp-lang/issues/11#issue-3006787260)
Detailed planning documents for core system features:
- [`01-frontend.md`](https://github.com/sefunmi4/qpp-lang/blob/main/docs/architecture/01-frontend.md#q-frontend-specification) – Grammar, syntax, and parser expectations
- [`02-runtime.md`](https://github.com/sefunmi4/qpp-lang/blob/main/docs/architecture/02-runtime.md) – Runtime architecture: `qalloc`, `task<>`, `qguard`
- [`03-hardware-api.md`](https://github.com/sefunmi4/qpp-lang/blob/main/docs/architecture/03-hardware-api.md) – Planned integration with QPUs

Each file links directly to GitHub issues and project boards. These are meant for experienced contributors focused on design, implementation details, or tracking bugs.

---

### 🧠 `./migration/` – Language Design Concepts [#12](https://github.com/sefunmi4/qpp-lang/issues/12#issue-3006787399)
Documents explaining how Q++ departs from or builds on traditional C++:
- `struct_vs_qstruct.md` – Modeling quantum structures
- `bool_logic.md` – Probabilistic truth vs binary logic
- `register_mapping.md` – qregister and cregister allocation rules
- `function_tasks.md` – Execution models for `task<QPU>`, `task<CPU>`, etc.
- `bitwise_to_gate.md` – Gate mappings from classical bitwise ops
- `asm_vs_qasm.md` – Low-level quantum gate injection
- `ir_and_probabilities.md` – Collapse tracking, IR output, and QIR

These docs are ideal for new contributors and those trying to understand Q++'s philosophy, compiler decisions, or runtime behavior. They can also help identify *expected behavior* and how features were meant to work.

---

### 🧪 `./examples/` – Playable Q++ Code [#13](https://github.com/sefunmi4/qpp-lang/issues/13#issue-3006787477)
Reference `.qpp` files that demonstrate working language features:
- `adaptive_task.qpp` – Conditional branching and task adaptation
- `hello_world.qpp` – Minimal task and print interaction
- `teleport.qpp` – Quantum teleportation and entanglement usage
- `demo.qpp` – Quick demonstration to compile and run with `qppc` and `qpp-run`
- `wavefunction_demo.qpp` – Exercises all built-in gates and conditional
  measurement logic
- `bitwise_demo.qpp` – Shows bitwise operators expanding to quantum gates


Use these to test local builds or see practical use cases of language features.

---

## 💡 Developer Workflow [#14](https://github.com/sefunmi4/qpp-lang/issues/14#issue-3006787636)

- **New Developers** → Start with `./migration/` and `./intro/` to understand Q++ philosophy.
  - Check `./examples/` to see how it looks in code.
  - Find related implementation plans in `./architecture/`.
  - Explore linked GitHub issues to learn what still needs work.

- **Experienced Developers** → Focus on `./architecture/` to help shape functionality.
  - Link your PRs to the matching planning doc and issues.
  - Document changes and edge cases clearly.

---

## 🔗 GitHub Integration [#15](https://github.com/sefunmi4/qpp-lang/issues/15#issue-3006787689)

Each doc in `/architecture/` links to its tracked issues. We recommend:
- Using GitHub Issue IDs in doc comments
- Referencing last-known bugs or discussion threads inside the MD files
- Keeping these files updated as implementation evolves

This makes Q++ easier to maintain and evolve, and much easier to onboard into.

---

## 🌱 Goal

Build Q++ with clarity and intention — one documented decision at a time.

