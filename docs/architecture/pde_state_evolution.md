# PDE-Based State Evolution

## \U0001F3AF Motivation
For very long sequences of gates, simulating state evolution one gate at a time can be expensive. Research tasks propose estimating the evolution using well known relativistic wave equations. The Dirac and Klein–Gordon equations act as continuous models for how amplitudes propagate through space and time.

## \U0001F4C8 Dirac Equation Approach
The one‑dimensional Dirac equation is written as

```
(i\hbar \partial_t - m c^2 \beta - c \alpha \partial_x) \psi(x,t) = 0
```

where `\alpha` and `\beta` are the Pauli matrices. Discretising the spatial derivative and stepping forward in time lets us approximate the evolution of each amplitude `\psi(x,t)`.

A small finite‑difference loop can be integrated into the wavefunction simulator:

```cpp
std::vector<std::complex<double>> psi(N);
for (size_t t = 0; t < steps; ++t) {
    for (size_t i = 1; i < N - 1; ++i) {
        auto dpsi = (psi[i + 1] - psi[i - 1]) / (2 * dx);
        psi[i] += (-i * c * dpsi - i * m * c * c / hbar * psi[i]) * dt;
    }
}
```

This provides a coarse approximation of how a relativistic particle’s state evolves without explicitly applying every gate.

## \U0001F30C Klein–Gordon Option
The Klein–Gordon equation offers another scalar field model:

```
(\Box + (mc/\hbar)^2) \phi(x,t) = 0
```

Using a similar discretisation of the d’Alembert operator `\Box`, we can evolve a scalar amplitude field `\phi` over time. This is useful for scenarios where spin is ignored.

## \U0001F527 Integration Points
- Use these PDE solvers when a circuit’s depth exceeds a configurable threshold.
- Propagate boundary conditions from the current `Wavefunction` state.
- Sample the resulting field back into qubit amplitudes before further gate operations.

These techniques are still experimental but can dramatically reduce simulation time for deep circuits.
