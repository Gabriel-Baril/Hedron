workspace "Hedron"
	architecture "x64" -- Chooses the architecture of our workspace (solution)

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

include "Hedron/vendor/GLFW" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/GLFW" in this premake file

project "Hedron"      -- Name of the project
	location "Hedron" -- Location of our stuff
	kind "SharedLib"  -- Type of project (in that case a dll (dynamic library))
	language "C++"    -- The language in which the project is written

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")      -- Output dir for our binaries (.exe, .lib, .dll ...)
	objdir    ("bin-int/"  .. outputdir .. "/%{prj.name}") -- Output dir for our object file (.obj)
	
	pchheader "hdrpch.h"   -- What are our precompiled header
	pchsource "Hedron/src/hdrpch.cpp"

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
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}


	vpathdestination = "src/Hedron/Events"
	vpathfiledestination = "Hedron/src/Hedron/Events"
	vpaths -- https://github.com/premake/premake-core/wiki/vpaths
	{ 
		["%{vpathdestination}/ApplicationEvents/*"] =
		{ 
			"%{vpathfiledestination}/AppTickEvent.h",
			"%{vpathfiledestination}/AppUpdateEvent.h",
			"%{vpathfiledestination}/AppRenderEvent.h",
			"%{vpathfiledestination}/WindowCloseEvent.h",
			"%{vpathfiledestination}/WindowResizeEvent.h"
		},
		["%{vpathdestination}/KeyboardEvents/*"] =
		{ 
			"%{vpathfiledestination}/KeyPressedEvent.h",
			"%{vpathfiledestination}/KeyReleasedEvent.h"
		},
		["%{vpathdestination}/MouseEvents/*"] =
		{
			"%{vpathfiledestination}/MouseMovedEvent.h",
			"%{vpathfiledestination}/MouseScrolledEvent.h",
			"%{vpathfiledestination}/MouseButtonPressedEvent.h",
			"%{vpathfiledestination}/MouseButtonReleasedEvent.h"
		},
		["src/Hedron/Events/Common/*"] =
		{
			"%{vpathfiledestination}/Event.h",
			"%{vpathfiledestination}/KeyEvent.h",
			"%{vpathfiledestination}/MouseButtonEvent.h"
		}
	}

	filter "system:windows" -- The instructions downward are only applied for windows users
		cppdialect "C++17"  -- The version of our C++ used
		staticruntime "On"  -- Do we want to link runtime libraries (Yes)
		systemversion "latest" -- Chooses the windows version of our project

		defines -- Defines preprocessor definition
		{
			"HDR_PLATFORM_WINDOWS",
			"HDR_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines 
		{
			"HDR_DEBUG",
			"HDR_ENABLE_ASSERTS"
		}
		symbols "On"
		
	filter "configurations:Release"
		defines "HDR_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "HDR_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"Hedron/src"
	}

	links
	{
		"Hedron"
	}

	filter "system:windows" -- The instructions downward are only applied for windows users
		cppdialect "C++17"  -- The version of our C++ used
		staticruntime "On"  -- Do we want to link runtime libraries (Yes)
		systemversion "latest" -- Chooses the windows version of our project

		defines -- Defines preprocessor definition
		{
			"HDR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HDR_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "HDR_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "HDR_DIST"
		optimize "On"