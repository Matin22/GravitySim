# GravitySim

A real-time 3D N-body gravity simulator written in C++ and OpenGL. Bodies attract each other using Newtonian gravity, and you can fly around the scene with a free camera and fire new masses into the system.

## Features

- Real-time N-body gravitational simulation
- Free-fly 3D camera with mouse look and scroll zoom
- Fire new balls into the simulation and watch them interact
- Object trails showing recent motion
- A toggleable 3D reference grid that warps to visualize the gravity well

## Requirements

- CMake 3.21 or newer
- A C++17 compiler (MSVC, GCC, or Clang)
- OpenGL drivers

GLFW and GLM are fetched automatically by CMake. GLAD is bundled in the repository.

## Building

```bash
git clone https://github.com/Matin22/GravitySim.git
cd GravitySim
cmake -B build
cmake --build build --config Release
```

The executable is placed in the build output directory alongside a copy of the `res` folder it needs at runtime.

## Controls

| Input | Action |
| --- | --- |
| `W` / `A` / `S` / `D` | Move camera |
| `Space` / `Left Shift` | Move camera up / down |
| Mouse | Look around |
| Scroll wheel | Zoom |
| Left click | Fire a new ball from the camera |
| `G` | Toggle the reference grid |
| `Esc` | Quit |

## Configuration

Simulation parameters such as gravitational constant, starting bodies, shot speed/mass, and trail settings can be adjusted in [src/conf.hpp](src/conf.hpp).

## License

Licensed under the [MIT License](LICENSE).
