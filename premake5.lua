workspace "OpenGL"
	architecture "x64"
	startproject "OpenGL"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"]  = "OpenGL/vendor/GLFW/include"
	IncludeDir["GLAD"]  = "OpenGL/vendor/GLAD/include"
	IncludeDir["ImGui"] = "OpenGL/vendor/imgui"
	IncludeDir["glm"]   = "OpenGL/vendor/glm"
	IncludeDir["stb_image"] = "OpenGL/vendor/stb_image"


	group "Dependencies"

		include "OpenGL/vendor/GLFW"
		include "OpenGL/vendor/GLAD"
		include "OpenGL/vendor/ImGui"

	
	group "Core-OpenGL"
	    project "OpenGL"
		location "OpenGL"
		kind "ConsoleApp"

		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir("bin/" .. outputdir .. "/%{prj.name}")
		objdir("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/stb_image/**.cpp",
			"%{prj.name}/vendor/stb_image/**.h",

		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/GLFW/include",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb_image}"
		}

		links
		{
			"GLFW",
			"GLAD",
			"ImGui",
			"opengl32.lib"
		}

		filter "system.windows"
			systemversion "latest"

		filter "configurations:Debug"
			defines "GL_Debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "GL_RELEASE"
			runtime "Release"
			symbols "on"
