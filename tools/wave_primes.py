#!/usr/bin/env python3
"""Proof-of-concept prime detection using wavefunction interference.

This script demonstrates how square and cosine wave superposition can
highlight prime and composite numbers. It does **not** implement a real
quantum algorithm but serves as a classical approximation inspired by
quantum wavefunction behavior.
"""

import argparse
import numpy as np
from scipy.signal import square
import matplotlib.pyplot as plt


def generate_wave(limit: int, method: str):
    """Generate a composite wave for all primes up to ``limit``.

    Parameters
    ----------
    limit : int
        Highest integer to examine.
    method : str
        Either ``"square"`` or ``"cosine"``.

    Returns
    -------
    x : ndarray
        Sequence of integers ``1..limit``.
    wave : ndarray
        Composite wave amplitude at each ``x``.
    primes : list[int]
        List of prime numbers used in the sum.
    """
    x = np.arange(1, limit + 1)
    wave = np.zeros_like(x, dtype=float)
    primes = []
    for n in range(2, limit + 1):
        if all(n % p != 0 for p in primes):
            primes.append(n)
            if method == "square":
                wave += square(2 * np.pi * x / n)
            else:
                # Phase shift so zeros coincide with multiples of ``n``
                wave += np.cos(2 * np.pi * x / n + np.pi)
    return x, wave, primes


def main() -> None:
    ap = argparse.ArgumentParser(description="Prime wavefunction demo")
    ap.add_argument("-n", "--limit", type=int, default=50,
                    help="upper bound for the wave range")
    ap.add_argument("--method", choices=["square", "cosine"], default="square",
                    help="type of wave to use")
    ap.add_argument("--plot", action="store_true",
                    help="display the resulting wave plot")
    args = ap.parse_args()

    x, wave, primes = generate_wave(args.limit, args.method)
    print("Primes used:", primes)

    if args.plot:
        plt.figure(figsize=(8, 4))
        plt.plot(x, wave, label="composite wave")
        plt.xlabel("Integer")
        plt.ylabel("Amplitude")
        plt.title(f"{args.method.capitalize()} wave prime pattern")
        plt.grid(True)
        plt.legend()
        plt.show()
    else:
        for value, amp in zip(x, wave):
            print(f"{value:3d}: {amp: .3f}")


if __name__ == "__main__":
    main()
