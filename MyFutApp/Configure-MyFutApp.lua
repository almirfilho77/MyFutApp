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
        "%{IncludeDir.CatolYeah}/vendor/imgui",
        "%{IncludeDir.opencv}",
        "%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-1.19.2/include",
        -- "%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-gpu-1.19.2/include",
    }

    libdirs
    {
        "%{wks.location}/vendor/opencv/lib",
        "%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-1.19.2/lib",
        -- "%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-gpu-1.19.2/lib",
    }
    
    postbuildcommands 
    { 
        "{COPYFILE} %[%{wks.location}/vendor/opencv/lib/opencv_world490d.dll] %[%{cfg.targetdir}/opencv_world490d.dll]",
        "{COPYFILE} %[%{wks.location}/vendor/ffmpeg/opencv_videoio_ffmpeg490_64.dll] %[%{cfg.targetdir}/opencv_videoio_ffmpeg490_64.dll]",
        "{COPYFILE} %[%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-1.19.2/lib/onnxruntime.dll] %[%{cfg.targetdir}/onnxruntime.dll]",
        -- "{COPYFILE} %[%{wks.location}/vendor/onnxruntime/onnxruntime-win-x64-gpu-1.19.2/lib/onnxruntime.dll] %[%{cfg.targetdir}/onnxruntime.dll]",
        -- "{COPYFILE} %[%{prj.location}/assets/yolov8m_fp16.onnx] %[%{cfg.targetdir}/yolov8m_fp16.onnx]",
        "{COPYFILE} %[%{prj.location}/assets/yolov8m.onnx] %[%{cfg.targetdir}/yolov8m.onnx]",
        "{COPYFILE} %[%{prj.location}/assets/coco.yaml] %[%{cfg.targetdir}/coco.yaml]"
    }

    links
    {
        "CatolYeah",
        "opencv_world490d.lib",
        "onnxruntime.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CY_PLATFORM_WINDOWS",
            -- "USE_CUDA"
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