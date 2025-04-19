# Q++ Runtime Specification

## ⚙️ Overview
The Q++ runtime layer is responsible for executing hybrid classical-quantum programs compiled from Q++ source code. It manages task scheduling, quantum-classical bit resolution, deferred execution of probabilistic scopes, measurement collapse, and device targeting (QPU or CPU).

This document outlines the architecture and expected behavior of the runtime layer, from initial dispatch to wavefunction simulation or QPU execution.

---

## 🎮 Execution Model

### Task Execution Contexts [🔗 Issue #201](https://github.com/qpp-lang/qpp/issues/201)
Q++ defines three runtime task contexts:
- `task<CPU>` → Runs directly on CPU
- `task<QPU>` → Delegated to quantum processing unit
- `task<AUTO>` → Evaluated and dispatched based on scope and data

Runtime determines target execution via scope analysis, bit type resolution, and probabilistic annotations.

### Probabilistic Tagging [🔗 Issue #202](https://github.com/qpp-lang/qpp/issues/202)
Scopes with quantum conditionals (e.g., `if (q[0])`) are tagged as probabilistic. These scopes:
- Cannot be resolved until measurement or simulation
- Must be tracked by the runtime for quantum resolution

### Scope Resolution Flow [🔗 Issue #203](https://github.com/qpp-lang/qpp/issues/203)
1. Parse task and determine execution path
2. Check for probabilistic tags
3. If none → execute immediately
4. If tagged → defer to simulator or QPU

---

## 🔄 Simulation Layer

### Wavefunction Simulator [🔗 Issue #204](https://github.com/qpp-lang/qpp/issues/204)
Used when real QPU is not available or during testing:
- Tracks all quantum states
- Resolves conditionals using amplitude sampling
- Emits measurement collapse when required

### Collapse API [🔗 Issue #205](https://github.com/qpp-lang/qpp/issues/205)
```cpp
collapse(q[1]);
```
- Collapses state and resolves all upstream probabilistic scopes
- Required to finalize any deferred logic branches

### Timeout Conditions [🔗 Issue #206](https://github.com/qpp-lang/qpp/issues/206)
Quantum states exceeding coherence time or simulator bounds raise:
- `EntanglementTimeoutError`
- `UncomputablePathError`

---

## 📦 Memory and Bit Models

### Bit Inference Model [🔗 Issue #207](https://github.com/qpp-lang/qpp/issues/207)
- Default: All variables are quantum unless explicitly marked `cbits`
- Quantum and classical bits co-exist in memory but follow different dispatch logic

### Register Mapping [🔗 Issue #208](https://github.com/qpp-lang/qpp/issues/208)
- `register` resolves to a CPU or QPU memory bank depending on context
- Developers may force register type using `cregister` or `qregister`

---

## 🔧 Task Scheduling and Dispatch

### Hybrid Dispatch Queue [🔗 Issue #209](https://github.com/qpp-lang/qpp/issues/209)
- `AUTO` tasks enter a dispatch queue
- Runtime analyzes scope, calls, and bit usage
- Dispatched to either CPU, simulator, or QPU

### Hardware Capabilities Map [🔗 Issue #210](https://github.com/qpp-lang/qpp/issues/210)
Defines available QPUs, simulators, and constraints like:
- Qubit count
- Gate limits
- Coherence windows

---

## 🧪 Debugging and Logging

### Probabilistic Trace Logging [🔗 Issue #211](https://github.com/qpp-lang/qpp/issues/211)
- Log probabilistic branches
- Output execution paths taken
- Show amplitude weights

### Scope Map Output [🔗 Issue #212](https://github.com/qpp-lang/qpp/issues/212)
- Visual output of tasks with color-coded scope states
- Indicates resolved, collapsed, deferred states

---

## 📌 Notes for Implementation
- Use wavefunction simulation backend for dev
- Support backend-agnostic dispatch APIs
- Emit IR with `@collapse_marker` or similar tags for QPU vendors
- Final hardware support via IBM Q / Google Cirq APIs

---

*End of Runtime Spec v0.1*

