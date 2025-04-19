# Q++ Hardware API Specification

## 🧬 Overview
The Q++ Hardware API defines how compiled programs are dispatched to real quantum and classical devices. It manages communication between Q++ runtime, simulators, QPUs (e.g., IBM Q, Google Sycamore), and future custom hardware. This document outlines how quantum instructions, register allocations, collapse metadata, and hardware constraints are translated and mapped at runtime.

---

## 🔌 QPU Integration Targets

### IBM Q Integration 
- Use IBM Qiskit backend APIs
- Translate IR to OpenQASM-compatible format
- Handle authentication and queueing

### Google Sycamore Integration 
- Use Cirq as target for gate translation
- Handle device connectivity maps and constraints
- Optimize for gate depth and decoherence

### Simulated Hardware
- Wavefunction backend for testing
- Mirror same API calls as real hardware

---

## 🧠 Register and Memory Abstractions

### Register Mapping Layer
- `qregister`, `cregister`, and generic `register` resolve at compile-time
- Hardware mapping respects QPU register limits
- Emit errors if over-allocated

### Collapse Marker Emission
- QIR or LLVM IR extended with `@collapse_marker`
- Used to denote measurement boundaries
- Allows QPU API to resolve conditional branches

---

## 🧭 Device Capability Mapping

### Hardware Profile Format
```json
{
  "device_id": "ibmq_tokyo",
  "max_qubits": 20,
  "supported_gates": ["X", "CX", "H", "T"],
  "coherence_time_us": 120,
  "max_depth": 500
}
```
- Defines compile-time target constraints
- Used by Q++ optimizer and scheduler

### Target Selection
- Runtime selects best-fit device based on:
  - Number of qubits
  - Gate availability
  - Probabilistic scope complexity

---

## ⚡ API Dispatch Flow

### Dispatch Function
```cpp
send_to_qpu(TaskIR task, HardwareProfile profile);
```
- Translates internal IR to QPU-specific job format
- Waits for result or error callback

### Return Types and Collapse
- QPU response includes:
  - Final bitstring or collapsed state
  - Error rate or confidence range
  - Metadata trace (optional)

---

## 🔐 Authentication and Rate Limits

### QPU API Credentials
- Allow token/secret registration via `.qppconfig`
- Support scoped access for testing vs production

### Rate Limit Detection
- Automatically fallback to simulator when:
  - QPU is overloaded
  - Exceeds monthly quota

---

## 🧪 Testing with Emulated Hardware

### Emulated Backend Harness
- Implements same API as real devices
- Logs full collapse paths and amplitude histories
- Can replay jobs and visualize interference

---

## 🌐 Future Device Support
- Custom ASIC/QPU API bridge
- FPGA accelerated quantum simulation
- Edge-device compatible quantum logic

---

*End of Hardware API Spec v0.1*

