function getWorkspacePath()
    local currentDir = _SCRIPT
    while currentDir do
        local directory = path.getdirectory(currentDir)
        if os.isfile(path.join(directory, "premake5.lua")) then
            return directory
        end
        currentDir = directory
    end
    return nil
end

workspace "LearnOpenGL"
    architecture "x64"
    configurations {"Debug", "Release"}

    outputfold = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    topfold = getWorkspacePath()

    startproject "Sandbox"

    include "Vendor/glfw"
    include "Vendor/glad"
    include "Core"
    include "Sandbox"
    include "Tests"