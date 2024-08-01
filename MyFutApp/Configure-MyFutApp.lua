-- Configure-MyFutApp.lua

project "MyFutApp"
    kind "ConsoleApp"
    staticruntime "on"
    language "C++"
    cppdialect "C++17"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.CatolYeah}/src",
        "%{IncludeDir.CatolYeah}/vendor/spdlog/include",
        "%{IncludeDir.CatolYeah}/vendor/glm/",
        "%{IncludeDir.CatolYeah}/vendor/imgui"
    }

    links
    {
        "CatolYeah"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CY_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CY_CONFIG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CY_CONFIG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "CY_CONFIG_DIST"
        runtime "Release"
        optimize "on"