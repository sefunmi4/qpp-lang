# Project TODOs

The following tasks were outlined before open-sourcing and are now mostly addressed:

- Basic parser and IR generation in `qppc`.
- IR execution via `qpp-run` using the runtime scheduler and memory manager.
- Thread-safe `MemoryManager` and `Scheduler` with simple error handling.
- Extra quantum gates (S, T, SWAP) added to the `Wavefunction` class.
- Expanded test suite covering additional gates and multi-threaded memory use.
- Initial contribution guidelines and issue templates.

Further work is needed to refine the IR format, improve error diagnostics, and flesh out compiler backends.

## Help Wanted

The following items could use community help. Tasks tagged `good-first-issue`
are relatively self-contained and ideal for newcomers.

- `good-first-issue` Extend `Scheduler` with task priorities and async execution.
- `good-first-issue` Add register reuse logic to `MemoryManager` to avoid excess
  allocations.
- Add support for saving/loading `QRegister` state for interoperability with
  external simulators.
- Improve diagnostics throughout `qppc` and `qpp-run`, surfacing clearer error
  messages.
