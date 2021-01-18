project "Poly"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"  -- The version of our C++ used
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")      -- Output dir for our binaries (.exe, .lib, .dll ...)
    objdir    ("bin-int/"  .. outputdir .. "/%{prj.name}") -- Output dir for our object file (.obj)

    files -- Chooses the files that we want to add in our project
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs -- Adds additional include folders
    {
        "%{wks.location}/Hedron/src",
        "%{includeDir.spdlog}",
        "%{includeDir.glm}",
        "%{includeDir.imgui}",
        "%{includeDir.entt}",
        "%{includeDir.ImGuizmo}"
    }

    links
    {
        "Hedron"
    }

    filter "system:windows" -- The instructions downward are only applied for windows users
        staticruntime "on"  -- Do we want to link runtime libraries (Yes)
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
