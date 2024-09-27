project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    toolset "v143"
    systemversion "latest"

    targetdir (path.join(topfold, "bin", outputfold, "%{prj.name}"))
    objdir (path.join(topfold, "bin-obj", outputfold, "%{prj.name}"))

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
    }

    filter "configurations:Debug"
        symbols "On"         -- 开启调试符号
        optimize "Off"       -- 关闭优化
        postbuildcommands
        {
            "{COPYFILE} " .. path.join(topfold, "bin", outputfold) .. "/assimp/assimp-vc143-mtd.dll " .. path.join(topfold, "bin", outputfold, "%{prj.name}"),
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"        -- 开启优化
        postbuildcommands
        {
            "{COPYFILE} " .. path.join(topfold, "bin", outputfold) .. "/assimp/assimp-vc143-mt.dll " .. path.join(topfold, "bin", outputfold, "%{prj.name}")
        }

    filter {}