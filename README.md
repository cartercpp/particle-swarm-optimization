# Particle Swarm Optimization

A from-scratch C++23 implementation of particle swarm optimization (PSO), with a live terminal visualization.

Part of [ml-from-scratch](https://github.com/cartercpp/ml-from-scratch).

## What it does

PSO is a population-based optimizer. Each particle has a position and velocity on the search space. Every step, velocity is pulled toward:

- the particle's own best position so far
- the swarm's global best position

This demo minimizes `sin(1 / x)` on `[0.1, 0.6]` with 50 particles in one dimension. The function is drawn in cyan; particles are red `o` markers. The animation speeds up as frames pass. Press Enter to quit.

Update rule used here:

```text
v = w * v + c1 * r1 * (pbest - x) + c2 * r2 * (gbest - x)
x = clamp(x + v, minX, maxX)
```

Defaults:

| Parameter | Value |
|-----------|-------|
| Swarm size | 50 |
| Inertia weight `w` | 0.7 |
| Personal weight `c1` | 1.5 |
| Global weight `c2` | 1.5 |
| Domain | `[0.1, 0.6]` |

## Build and run

Needs a C++23 compiler and CMake.

```bash
cmake -S . -B build
cmake --build build
./build/ParticleSwarmOptimization
```

Use a terminal that supports ANSI colors and is at least about 170 columns wide for the full plot.

## Files

- `main.cpp` — particle struct, PSO loop, and ASCII renderer
- `CMakeLists.txt` — C++23 executable target `ParticleSwarmOptimization`
