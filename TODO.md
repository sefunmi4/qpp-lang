# Project TODOs

The following tasks were outlined before open-sourcing and are now mostly addressed:

- Basic parser and IR generation in `qppc`.
- IR execution via `qpp-run` using the runtime scheduler and memory manager.
- Thread-safe `MemoryManager` and `Scheduler` with simple error handling.
- Extra quantum gates (S, T, SWAP) added to the `Wavefunction` class.
- Expanded test suite covering additional gates and multi-threaded memory use.
- Initial contribution guidelines and issue templates.

Further work is needed to refine the IR format, improve error diagnostics, and flesh out compiler backends.

