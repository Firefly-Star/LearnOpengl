workspace "LearnOpenGL"
    architecture "x64"
    configurations {"Debug", "Release"}

    outputfold = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    startproject "Sandbox"

    include "Vendor/glfw"
    include "Vendor/glad"
    include "Core"
    include "Sandbox"