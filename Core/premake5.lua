project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"
    toolset "v143"

    targetdir ("bin/" .. outputfold .. "/%{prj.name}")
    objdir("obj/" .. outputfold .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/**.h",
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
        "../Vendor/assimp/bin/" .. outputfold .. "/assimp"
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