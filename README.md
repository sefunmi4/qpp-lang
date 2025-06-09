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
| `task<T>` | `task<QPU>`, `task<CPU>`, `task<AUTO>`, `task<MIXED>` target hardware or hybrid modes |
| `__qasm` | Inject raw gate-level code (like inline asm) |
| LLVM IR | Enhanced with QIR, collapse metadata, and probabilistic flags |
| Import/Export | Save and restore `qregister` state for external simulators |
| Scheduler | Priorities, async run, pause and stop controls |
| Hardware API | Emits QIR strings and plugs into vendor backends. Stubs for Qiskit, Cirq, Braket, Q#, NVIDIA, and PsiQuantum are included; real SDK integration is still required. |

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
- Simulate CPU/QPU behavior in a fallback runtime with a prioritized
  asynchronous scheduler

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

### 🧪 Wavefunction Prime Demo

We now include `tools/wave_primes.py`, a short Python script that explores
finding primes via interference patterns. Use it to plot the composite wave for
the first few primes:

```bash
python tools/wave_primes.py --plot --method square -n 50
```

The demo is purely experimental but serves as a playground for ideas inspired by
quantum Fourier transforms.

### 🌌 Spacetime Slicing Demo

`tools/spacetime_slicing.py` visualizes a wavefunction expanding radially over
time. Each angular segment represents a possible timeline branch. Launch an
animation with:

```bash
python tools/spacetime_slicing.py --animate --frames 80
```

Use `--time` instead of `--animate` to view a single snapshot at a specific
moment.



## Building and Testing

The project uses CMake. A typical build workflow is:

```bash
mkdir build && cd build
cmake ..
make
ctest
```

`ctest` executes the small wavefunction simulator tests.

The wavefunction runtime now supports low-precision `float` amplitudes via a
template parameter. All examples and unit tests build against this mode.

### Python Requirements

To run the demo in `tools/wave_primes.py` install dependencies via:

```bash
pip install -r requirements.txt
```

`qiskit` is optional but required for the experimental hardware backend.

For contribution guidelines see [CONTRIBUTING.md](CONTRIBUTING.md).

### Random Number Generation

Utilities in `runtime/random.h` manage pseudo-random sequences used by the
wavefunction simulator. `global_rng()` returns a thread-local `std::mt19937`
engine seeded from an atomic value. Use `seed_rng()` to set this seed for
deterministic behavior. The runtime relies on `global_rng()` when collapsing
qubits during measurement. The unit tests in `/tests` demonstrate seeding the
generator with a fixed value before running simulations.

### Quick Start Example

`qppc` now parses a small but useful subset of Q++ including simple
`if`/`else` conditionals and backend hint annotations such as `@clifford`
and `@dense`. Gate application and measurement remain the primary focus
while the frontend evolves.

Compile and run the sample program in `docs/examples/demo.qpp` with:

```bash
qppc docs/examples/demo.qpp demo.ir
qpp-run demo.ir
```
You can optionally supply a hardware profile file to enforce device limits:
```bash
qppc docs/examples/demo.qpp demo.ir --profile ibmq.json
```

This demonstrates the toy toolchain using the runtime scheduler and wavefunction simulator.

For a more thorough test of the simulator, try `docs/examples/wavefunction_demo.qpp`.
This example has been simplified to use only gates and measurements so it
builds cleanly with the current compiler:

```bash
qppc docs/examples/wavefunction_demo.qpp wf.ir
qpp-run wf.ir
```

The decision diagram implementation can be benchmarked using:

```bash
quidd_benchmark 8
```

This compares memory usage of the dense wavefunction against the QuIDD form.

To see how bitwise operators map to quantum gates, compile `docs/examples/bitwise_demo.qpp`:

```bash
qppc docs/examples/bitwise_demo.qpp bitwise.ir
qpp-run bitwise.ir
```

If `qiskit` is installed you can run the same IR on the Qiskit simulator by registering the backend:

```bash
qpp-run --use-qiskit demo.ir
```

If you have external frameworks installed you can run the IR on different simulators or hardware by registering another backend:

```bash
qpp-run --use-qiskit demo.ir    # IBM Qiskit
qpp-run --use-cirq demo.ir      # Google/Cirq
qpp-run --use-braket demo.ir    # Amazon Braket
qpp-run --use-qsharp demo.ir    # Microsoft Q#
qpp-run --use-nvidia demo.ir    # NVIDIA
qpp-run --use-psi demo.ir       # PsiQuantum
```

By default the runtime executes on the CPU. Pass `--device GPU` to `qpp-run`
to request GPU kernels when built with CUDA support. If CUDA is unavailable the
tool falls back to the CPU implementation automatically.

### Open Tasks

See [TODO.md](TODO.md) for current areas where help is welcome. Items marked
`good-first-issue` are ideal starting points for new contributors. The file also
contains a **Longer Term Roadmap** describing features like a full compiler,
advanced scheduler, and hardware API integration that would move Q++ toward
production readiness.
For a complete list of proposed features and their implementation tasks,
refer to [features.yaml](features.yaml).
- The runtime provides a basic resonance zone cache via
  `memory.save_resonance_zone` and `memory.load_resonance_zone`.

### Editor Support

For syntax highlighting in VS Code, a minimal extension is provided under
`/vscode`. Launch VS Code with the extension enabled using:

```bash
code --extensionDevelopmentPath=vscode .
```

This registers the `.qpp` extension with basic grammar definitions so Q++
files render cleanly without C++ template errors.

