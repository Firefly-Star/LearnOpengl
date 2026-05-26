# LearnOpenGL

A personal project for learning OpenGL graphics programming with C++.

## Description

This project contains exercises and experiments following the LearnOpenGL tutorial series. It includes implementations of shaders, textures, lighting, and 3D rendering techniques.

## Structure

```
Core/       — Core engine and rendering abstractions
Sandbox/    — Test and demo applications
Tests/      — Unit tests
Vendor/     — Third-party libraries (GLFW, Glad, ImGui, glm, spdlog)
```

## Build

Requires premake5 or CMake, and a C++ compiler.

```bash
# Using premake5
./BuildPremake.bat

# Or using CMake
mkdir build && cd build && cmake ..
```

## Dependencies

- GLFW (windowing and input)
- Glad (OpenGL function loader)
- GLM (OpenGL mathematics)
- ImGui (GUI)
- spdlog (logging)
