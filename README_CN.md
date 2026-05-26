# LearnOpenGL

学习 OpenGL 图形编程的个人项目（C++）。

## 描述

该项目包含跟随 LearnOpenGL 教程系列完成的练习和实验，涵盖了着色器、纹理、光照和 3D 渲染技术。

## 结构

```
Core/       — 核心引擎和渲染抽象层
Sandbox/    — 测试和示例应用
Tests/      — 单元测试
Vendor/     — 第三方库 (GLFW, Glad, ImGui, glm, spdlog)
```

## 构建

需要 premake5 或 CMake，以及 C++ 编译器。

```bash
# 使用 premake5
./BuildPremake.bat

# 或使用 CMake
mkdir build && cd build && cmake ..
```

## 依赖

- GLFW（窗口和输入）
- Glad（OpenGL 函数加载器）
- GLM（OpenGL 数学库）
- ImGui（GUI）
- spdlog（日志）
