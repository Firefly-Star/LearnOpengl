project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    toolset "v143"
    systemversion "latest"

    targetdir ("bin/" .. outputfold .. "/%{prj.name}")
    objdir ("obj/" .. outputfold .. "/%{prj.name}")

    files
    {
        "**.cpp"
    }

    includedirs
    {
        "",
        "../Core/src",
        "../Vendor/glfw/include",
        "../Vendor/glad/include",
        "../Vendor/glm",
    }

    links
    {
        "Core",
        "opengL32",
    }

    filter "configurations:Debug"
        symbols "On"         -- 开启调试符号
        optimize "Off"       -- 关闭优化
        postbuildcommands
        {
            "{COPYFILE} ../Vendor/assimp/bin/" .. outputfold .. "/assimp/assimp-vc143-mtd.dll bin/" .. outputfold .. "/%{prj.name}"
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"        -- 开启优化
        postbuildcommands
        {
            "{COPYFILE} ../Vendor/assimp/bin/" .. outputfold .. "/assimp/assimp-vc143-mt.dll bin/" .. outputfold .. "/%{prj.name}"
        }

    filter {}

    