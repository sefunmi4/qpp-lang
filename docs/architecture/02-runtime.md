# Q++ Runtime Specification

## ⚙️ Overview
The Q++ runtime layer is responsible for executing hybrid classical-quantum programs compiled from Q++ source code. It manages task scheduling, quantum-classical bit resolution, deferred execution of probabilistic scopes, measurement collapse, and device targeting (QPU or CPU).

This document outlines the architecture and expected behavior of the runtime layer, from initial dispatch to wavefunction simulation or QPU execution.

---

## 🎮 Execution Model 

### Task Execution Contexts [Issue #3](https://github.com/sefunmi4/qpp-lang/issues/3#issue-3006616767)
Q++ defines three runtime task contexts:
- `task<CPU>` → Runs directly on CPU
- `task<QPU>` → Delegated to quantum processing unit
- `task<AUTO>` → Evaluated and dispatched based on scope and data

Runtime determines target execution via scope analysis, bit type resolution, and probabilistic annotations.

### Probabilistic Tagging [Issue #5](https://github.com/sefunmi4/qpp-lang/issues/5#issue-3006630677) 
Scopes with quantum conditionals (e.g., `if (q[0])`) are tagged as probabilistic. These scopes:
- Cannot be resolved until measurement or simulation
- Must be tracked by the runtime for quantum resolution

### Scope Resolution Flow 
1. Parse task and determine execution path
2. Check for probabilistic tags
3. If none → execute immediately
4. If tagged → defer to simulator or QPU

---

## 🔄 Simulation Layer

### Wavefunction Simulator 
Used when real QPU is not available or during testing:
- Tracks all quantum states
- Resolves conditionals using amplitude sampling
- Emits measurement collapse when required

### Collapse API
```cpp
collapse(q[1]);
```
- Collapses state and resolves all upstream probabilistic scopes
- Required to finalize any deferred logic branches

### Timeout Conditions 
Quantum states exceeding coherence time or simulator bounds raise:
- `EntanglementTimeoutError`
- `UncomputablePathError`

---

## 📦 Memory and Bit Models

### Bit Inference Model 
- Default: All variables are quantum unless explicitly marked `cbits`
- Quantum and classical bits co-exist in memory but follow different dispatch logic

### Register Mapping 
- `register` resolves to a CPU or QPU memory bank depending on context
- Developers may force register type using `cregister` or `qregister`

---

## 🔧 Task Scheduling and Dispatch

### Hybrid Dispatch Queue
- `AUTO` tasks enter a dispatch queue
- Runtime analyzes scope, calls, and bit usage
- Dispatched to either CPU, simulator, or QPU

### Device Selection and Fallbacks
The runtime can target GPU kernels when available. Use `set_device(DeviceType::GPU)`
or pass `--device GPU` to `qpp-run` to enable GPU offload. If the runtime or build
lacks CUDA support it automatically falls back to the CPU implementation so tests
and examples still run.

### Hardware Capabilities Map 
Defines available QPUs, simulators, and constraints like:
- Qubit count
- Gate limits
- Coherence windows

---

## 🧪 Debugging and Logging

### Probabilistic Trace Logging
- Log probabilistic branches
- Output execution paths taken
- Show amplitude weights

### Scope Map Output
- Visual output of tasks with color-coded scope states
- Indicates resolved, collapsed, deferred states

---

## 📌 Notes for Implementation
- Use wavefunction simulation backend for dev
- Support backend-agnostic dispatch APIs
- Emit IR with `@collapse_marker` or similar tags for QPU vendors
- Final hardware support via IBM Q / Google Cirq APIs

---

### Scheduler Overview
The reference runtime now supports task priorities and asynchronous execution.
Tasks added via `scheduler.add_task` are queued in priority order. `scheduler.run_async()`
launches a worker thread so CPU tasks can execute concurrently with QPU
simulation. When a `task<QPU>` handler is invoked, it operates on registers
managed by `MemoryManager` and can apply gates through the `QRegister`
interface. CPU tasks run standard C++ code and may interact with classical
registers, enabling hybrid workflows.

*End of Runtime Spec v0.1*

