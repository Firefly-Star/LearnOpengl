project "TestFrame"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    toolset "v143"
    staticruntime "off"
    systemversion "latest"

    targetdir (path.join(topfold, "bin", outputfold, "%{prj.name}"))
    objdir (path.join(topfold, "bin-obj", outputfold, "%{prj.name}"))

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "../Vendor/googleTest",
        "../Vendor/googleBenchmark"
    }

    libdirs
    {
        path.join(topfold, "bin", outputfold, "googleBenchmark")
    }

    links
    {
        -- benchmark
        "benchmark.lib",
    }

    defines
    {
        -- benchmark
        "HAVE_STD_REGEX",
        "HAVE_STEADY_CLOCK",
        "TEST_BENCHMARK_LIBRARY_HAS_NO_ASSERTIONS",
        "BENCHMARK_STATIC_DEFINE"
    }

    filter "configurations:Debug"
        symbols "On"         -- �������Է���
        optimize "Off"       -- �ر��Ż�


    filter "configurations:Release"
        symbols "Off"
        optimize "Full"        -- �����Ż�
        

    filter {}
    
    filter "system:windows"
        links
        {
            -- benchmark
            "shlwapi.lib",
        }
    
    filter {}