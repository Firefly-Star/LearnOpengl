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
        symbols "On"         -- �������Է���
        optimize "Off"       -- �ر��Ż�
        postbuildcommands
        {
            "{COPYFILE} ../Vendor/assimp/bin/" .. outputfold .. "/assimp/assimp-vc143-mtd.dll bin/" .. outputfold .. "/%{prj.name}"
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"        -- �����Ż�
        postbuildcommands
        {
            "{COPYFILE} ../Vendor/assimp/bin/" .. outputfold .. "/assimp/assimp-vc143-mt.dll bin/" .. outputfold .. "/%{prj.name}"
        }

    filter {}

    