
project "Hedron"      -- Name of the project
    kind "StaticLib"  -- Type of project (in that case a .lib (static library))
    language "C++"    -- The language in which the project is written
    cppdialect "C++17"  -- The version of our C++ used
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")      -- Output dir for our binaries (.exe, .lib, .dll ...)
    objdir    ("bin-int/"  .. outputdir .. "/%{prj.name}") -- Output dir for our object file (.obj)
    
    pchheader "hdrpch.h"   -- What is our precompiled header
    pchsource "src/hdrpch.cpp"

    files -- Chooses the files that we want to add in our project
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl"
    }

    includedirs -- Adds additional include folders
    {
        "src",
        "%{includeDir.spdlog}",
        "%{includeDir.glfw}",
        "%{includeDir.glad}",
        "%{includeDir.imgui}",
        "%{includeDir.glm}",
        "%{includeDir.stb_image}",
        "%{includeDir.entt}",
        "%{includeDir.yaml_cpp}"
    }

    links
    {
        "glfw",
        "glad",
        "imgui",
        "yaml-cpp",
        "opengl32.lib"
    }

    vpathhdrdest = "src/Hedron"
    vpathhdrfiledest = "Hedron/src/Hedron"
    vpatheventdest = "src/Hedron/Events"
    vpatheventfiledest = "Hedron/src/Hedron/Events"
    vpaths -- https://github.com/premake/premake-core/wiki/vpaths
    { 
        ["%{vpatheventdest}/ApplicationEvents/*"] =
        { 
            "%{vpatheventfiledest}/AppTickEvent.h",
            "%{vpatheventfiledest}/AppUpdateEvent.h",
            "%{vpatheventfiledest}/AppRenderEvent.h",
            "%{vpatheventfiledest}/WindowCloseEvent.h",
            "%{vpatheventfiledest}/WindowResizeEvent.h"
        },
        ["%{vpatheventdest}/KeyboardEvents/*"] =
        { 
            "%{vpatheventfiledest}/KeyPressedEvent.h",
            "%{vpatheventfiledest}/KeyReleasedEvent.h",
            "%{vpatheventfiledest}/KeyTypedEvent.h"
        },
        ["%{vpatheventdest}/MouseEvents/*"] =
        {
            "%{vpatheventfiledest}/MouseMovedEvent.h",
            "%{vpatheventfiledest}/MouseScrolledEvent.h",
            "%{vpatheventfiledest}/MouseButtonPressedEvent.h",
            "%{vpatheventfiledest}/MouseButtonReleasedEvent.h"
        },
        ["%{vpatheventdest}/Common/*"] =
        {
            "%{vpatheventfiledest}/Event.h",
            "%{vpatheventfiledest}/KeyEvent.h",
            "%{vpatheventfiledest}/MouseButtonEvent.h"
        }
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows" -- The instructions downward are only applied for windows users
        systemversion "latest" -- Chooses the windows version of our project

        defines -- Defines preprocessor definition
        {
            "HDR_PLATFORM_WINDOWS",
            "HDR_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines 
        {
            "HDR_DEBUG"
        }
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "HDR_RELEASE"
        runtime "Release"
        optimize "on"
        
    filter "configurations:Dist"
        defines "HDR_DIST"
        runtime "Release"
        optimize "on"
