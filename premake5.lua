workspace "Hedron"
    architecture "x64" -- Chooses the architecture of our workspace (solution)
    startproject "Sandbox"

    configurations
    {
        "Debug",   -- Used for testing, logging, debugging and have no compiler optimisation 
        "Release", -- Like the Debug configuration but with compiler optimisation on
        "Dist"     -- The distribution configuration have no logging and have compiler optimisation on
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
includeDir = {}
includeDir["GLFW"] = "Hedron/vendor/GLFW/include"
includeDir["GLAD"] = "Hedron/vendor/GLAD/include"
includeDir["ImGui"] = "Hedron/vendor/imgui"
includeDir["glm"] = "Hedron/vendor/glm"
includeDir["stb_image"] = "Hedron/vendor/stb_image"

group "Dependencies"
    include "Hedron/vendor/GLFW" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/GLFW" in this premake file
    include "Hedron/vendor/GLAD" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/GLAD" in this premake file
    include "Hedron/vendor/imgui" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/imgui" in this premake file
group ""

project "Hedron"      -- Name of the project
    location "Hedron" -- Location of our stuff
    kind "StaticLib"  -- Type of project (in that case a .lib (static library))
    language "C++"    -- The language in which the project is written
    cppdialect "C++17"  -- The version of our C++ used
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")      -- Output dir for our binaries (.exe, .lib, .dll ...)
    objdir    ("bin-int/"  .. outputdir .. "/%{prj.name}") -- Output dir for our object file (.obj)
    
    pchheader "hdrpch.h"   -- What are our precompiled header
    pchsource "Hedron/src/hdrpch.cpp"

    files -- Chooses the files that we want to add in our project
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs -- Adds additional include folders
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{includeDir.GLFW}",
        "%{includeDir.GLAD}",
        "%{includeDir.ImGui}",
        "%{includeDir.glm}",
        "%{includeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
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
        },
        ["%{vpathhdrdest}/Inputs/*"] =
        {
            "%{vpathhdrfiledest}/Input.h",
            "%{vpathhdrfiledest}/KeyCodes.h",
            "%{vpathhdrfiledest}/MouseButtonCodes.h"
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"  -- The version of our C++ used
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")      -- Output dir for our binaries (.exe, .lib, .dll ...)
    objdir    ("bin-int/"  .. outputdir .. "/%{prj.name}") -- Output dir for our object file (.obj)

    disablewarnings
    {
        "4251"
    }

    files -- Chooses the files that we want to add in our project
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs -- Adds additional include folders
    {
        "Hedron/vendor/spdlog/include",
        "Hedron/src",
        "%{includeDir.glm}",
        "%{includeDir.ImGui}"
    }

    links
    {
        "Hedron"
    }

    filter "system:windows" -- The instructions downward are only applied for windows users
        staticruntime "On"  -- Do we want to link runtime libraries (Yes)
        systemversion "latest" -- Chooses the windows version of our project

        defines -- Defines preprocessor definition
        {
            "HDR_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "HDR_DEBUG"
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
