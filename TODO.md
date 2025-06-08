# Project TODOs

The following tasks were outlined before open-sourcing and are now mostly addressed:

- Basic parser and IR generation in `qppc`.
- IR execution via `qpp-run` using the runtime scheduler and memory manager.
- Thread-safe `MemoryManager` and `Scheduler` with simple error handling.
- Extra quantum gates (S, T, SWAP) added to the `Wavefunction` class.
- Expanded test suite covering additional gates and multi-threaded memory use.
- Initial contribution guidelines and issue templates.

Further work is needed to refine the IR format, improve error diagnostics, and flesh out compiler backends.

### Longer Term Roadmap

The following larger tasks are required before Q++ reaches feature parity with existing quantum frameworks:

- Compiler front-end parses simple control flow and emits an intermediate representation used by `qpp-run`.
- `qpp-run` now executes that IR via the scheduler and wavefunction backend, supporting conditional gates.
- ~~Expand the runtime scheduler with task priorities and asynchronous execution, allowing CPU hooks for hybrid workflows.~~ *(done)*
- ~~Extend the `Wavefunction` library with additional gates and utilities for multi-qubit operations and state management.~~ *(done)*
- Hardware API now includes a Qiskit backend example but more vendor SDKs should be plugged in for real QPU execution.
- Continue documenting migration guides such as `bitwise_to_gate.md` and `bool_logic.md`.
- Grow the test suite to cover more runtime paths and edge cases. *(ongoing)*

## Help Wanted

The following items could use community help. Tasks tagged `good-first-issue`
are relatively self-contained and ideal for newcomers.

- ~~`good-first-issue` Extend `Scheduler` with task priorities and async execution.~~ *(done)*
- ~~`good-first-issue` Add register reuse logic to `MemoryManager` to avoid excess
  allocations.~~ *(done)*
- ~~Add support for saving/loading `QRegister` state for interoperability with
  external simulators.~~ *(done)*
- Improve diagnostics throughout `qppc` and `qpp-run`, surfacing clearer error
  messages.

