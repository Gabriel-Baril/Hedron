workspace "Hedron"
    architecture "x64" -- Chooses the architecture of our workspace (solution)
    startproject "Poly"

    configurations
    {
        "Debug",   -- Used for testing, logging, debugging and have no compiler optimisation 
        "Release", -- Like the Debug configuration but with compiler optimisation on
        "Dist"     -- The distribution configuration have no logging and have compiler optimisation on
    }

    flags
    {
    	"MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
includeDir = {}
includeDir["glfw"] = "%{wks.location}/Hedron/vendor/glfw/include"
includeDir["glad"] = "%{wks.location}/Hedron/vendor/glad/include"
includeDir["spdlog"] = "%{wks.location}/Hedron/vendor/spdlog/include"
includeDir["imgui"] = "%{wks.location}/Hedron/vendor/imgui"
includeDir["glm"] = "%{wks.location}/Hedron/vendor/glm"
includeDir["stb_image"] = "%{wks.location}/Hedron/vendor/stb_image"
includeDir["entt"] = "%{wks.location}/Hedron/vendor/entt/include"
includeDir["yaml_cpp"] = "%{wks.location}/Hedron/vendor/yaml-cpp/include"

group "Dependencies"
    include "Hedron/vendor/glfw" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/glfw" in this premake file
    include "Hedron/vendor/glad" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/glad" in this premake file
    include "Hedron/vendor/imgui" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/imgui" in this premake file
    include "Hedron/vendor/yaml-cpp" -- Includes the premake5.lua file contained in the folder "Hedron/vendor/yaml-cpp" in this premake file
group ""

include "Hedron"
include "Sandbox"
include "Poly"
