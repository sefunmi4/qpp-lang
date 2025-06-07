# Register Mapping

When migrating C++ code to Q++, register declarations map to specific memory
banks depending on context.

- `register` – inferred by the compiler; may become a classical or quantum
  register based on usage.
- `cregister` – forces classical storage; lives purely in CPU memory.
- `qregister` – allocates qubits in the simulator or on a QPU.

MemoryManager keeps separate pools for classical and quantum registers and errors
if an invalid ID is used or a register is freed twice.
