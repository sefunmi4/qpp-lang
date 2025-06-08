#!/usr/bin/env python3
"""Prototype timeline decomposition using radial models.

This module visualizes a simple radial wavefunction and slices its evolution
into angular timelines.  Time is treated as a radial coordinate ``r = ct``
while ``theta`` encodes phase or spatial displacement.
"""

from __future__ import annotations

import argparse
from typing import Tuple

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation


def psi(r: np.ndarray, theta: np.ndarray, t: float,
        amplitude: float = 1.0, k: float = 2 * np.pi,
        omega: float = 2 * np.pi) -> np.ndarray:
    """Prototype radial wavefunction.

    Parameters
    ----------
    r : ndarray
        Radial coordinate(s).
    theta : ndarray
        Angular coordinate(s).
    t : float
        Time value.
    amplitude : float, optional
        Wave amplitude.
    k : float, optional
        Radial wave number.
    omega : float, optional
        Angular frequency.

    Returns
    -------
    ndarray
        Complex amplitude sampled at ``(r, theta, t)``.
    """
    return amplitude * np.cos(k * r - omega * t + theta)


def _radial_grid(r_max: float, angular_steps: int,
                 radial_steps: int) -> Tuple[np.ndarray, np.ndarray]:
    theta = np.linspace(0.0, 2 * np.pi, angular_steps)
    r = np.linspace(0.0, r_max, radial_steps)
    return np.meshgrid(theta, r)


def plot_snapshot(t: float = 1.0, r_max: float = 5.0,
                  angular_steps: int = 180, radial_steps: int = 100) -> None:
    """Display a single wavefunction snapshot at time ``t``."""
    theta, r = _radial_grid(r_max, angular_steps, radial_steps)
    amp = psi(r, theta, t)

    fig, ax = plt.subplots(subplot_kw={"projection": "polar"})
    pcm = ax.pcolormesh(theta, r, amp, cmap="twilight_shifted", shading="auto")
    ax.set_title(f"Wavefunction snapshot t={t:.2f}")
    ax.set_ylim(0, r_max)
    fig.colorbar(pcm, ax=ax, label="Amplitude")
    plt.show()


def animate(frames: int = 60, r_max: float = 5.0, interval: int = 100,
            angular_steps: int = 180, radial_steps: int = 100) -> None:
    """Animate the radial wavefunction over ``frames`` time steps."""
    theta, r = _radial_grid(r_max, angular_steps, radial_steps)
    fig, ax = plt.subplots(subplot_kw={"projection": "polar"})
    pcm = ax.pcolormesh(theta, r, psi(r, theta, 0), cmap="twilight_shifted",
                        shading="auto")
    ax.set_ylim(0, r_max)
    fig.colorbar(pcm, ax=ax, label="Amplitude")

    def update(frame: int):
        t = frame / frames * r_max
        pcm.set_array(psi(r, theta, t).ravel())
        ax.set_title(f"t={t:.2f}")
        return pcm,

    anim = FuncAnimation(fig, update, frames=frames, interval=interval)
    plt.show()


def main() -> None:
    ap = argparse.ArgumentParser(description="Spacetime slicing visualization")
    ap.add_argument("--animate", action="store_true",
                    help="run an animation instead of a single snapshot")
    ap.add_argument("--frames", type=int, default=60,
                    help="number of frames in the animation")
    ap.add_argument("--time", type=float, default=1.0,
                    help="time value for a static snapshot")
    ap.add_argument("--r-max", type=float, default=5.0,
                    help="maximum radial distance")
    args = ap.parse_args()

    if args.animate:
        animate(frames=args.frames, r_max=args.r_max)
    else:
        plot_snapshot(t=args.time, r_max=args.r_max)


if __name__ == "__main__":
    main()
