project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    toolset "v143"

    targetdir (path.join(topfold, "bin", outputfold, "%{prj.name}"))
    objdir (path.join(topfold, "bin-obj", outputfold, "%{prj.name}"))

    files
    {
        "src/**.cpp",
        "src/**.h",
        "src/**.tpp",
        "vendor/src/stb/stb_image.cpp"
    }

    includedirs
    {
        "src",
        "../Vendor/glfw/include",
        "../Vendor/glad/include",
        "../Vendor/glm",
        "../Vendor/assimp/include",
        "vendor/src/stb",
    }

    libdirs
    {
        path.join(topfold, "bin", outputfold, "assimp")
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32",
    }

    filter "configurations:Debug"
        symbols "On"         -- 开启调试符号
        optimize "Off"       -- 关闭优化

        links
        {
            "assimp-vc143-mtd"
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"        -- 开启优化
        links
        {
            "assimp-vc143-mt"
        }

    filter {}