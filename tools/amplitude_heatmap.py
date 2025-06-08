#!/usr/bin/env python3
"""Visualize wavefunction amplitudes as 2D/3D plots."""
import argparse
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm


def load_state(path: str) -> np.ndarray:
    """Load complex amplitudes from a text or .npy file."""
    if path.endswith('.npy'):
        return np.load(path)
    data = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            line = line.replace(',', ' ')
            parts = line.split()
            if len(parts) < 2:
                continue
            data.append(float(parts[0]) + 1j * float(parts[1]))
    return np.array(data, dtype=complex)


def plot_state(state: np.ndarray, show3d: bool = False) -> None:
    """Plot amplitude magnitudes and phases."""
    mags = np.abs(state)
    phases = np.angle(state)
    n = int(round(np.log2(len(state))))
    if 2 ** n != len(state):
        raise ValueError("State vector length must be a power of 2")

    dx = 1 << ((n + 1) // 2)
    dy = 1 << (n // 2)
    mags_grid = mags.reshape(dy, dx)
    phase_grid = phases.reshape(dy, dx)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
    im1 = ax1.imshow(mags_grid, cmap='viridis')
    ax1.set_title('Amplitude Magnitude')
    plt.colorbar(im1, ax=ax1)
    im2 = ax2.imshow(phase_grid, cmap='twilight')
    ax2.set_title('Phase (radians)')
    plt.colorbar(im2, ax=ax2)

    if show3d:
        from mpl_toolkits.mplot3d import Axes3D  # noqa: F401
        xs, ys = np.meshgrid(np.arange(dx), np.arange(dy))
        fig3d = plt.figure(figsize=(8, 6))
        ax = fig3d.add_subplot(111, projection='3d')
        ax.bar3d(xs.ravel(), ys.ravel(), np.zeros_like(mags_grid).ravel(),
                 1, 1, mags_grid.ravel(),
                 color=cm.twilight((phase_grid.ravel() + np.pi) / (2 * np.pi)))
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('|amp|')
        ax.set_title('3D Amplitude Magnitude with Phase Color')
    plt.show()


def main() -> None:
    ap = argparse.ArgumentParser(description='Amplitude heatmap visualizer')
    ap.add_argument('state', help='Path to text or .npy file with complex amplitudes')
    ap.add_argument('--3d', dest='show3d', action='store_true',
                    help='display 3D bar plot as well')
    args = ap.parse_args()
    state = load_state(args.state)
    plot_state(state, args.show3d)


if __name__ == '__main__':
    main()
