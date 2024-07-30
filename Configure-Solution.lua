-- Configure-Solution.lua

workspace "MyFutApp"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "CatolYeah/Configure-CatolYeah-External.lua"

-- Include dirs relative to root folder
IncludeDir["CatolYeah"] = "%{wks.location}/CatolYeah/CatolYeah"

group "App"
    include "MyFutApp/Configure-MyFutApp.lua"
group ""