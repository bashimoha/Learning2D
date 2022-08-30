workspace "Learning2D"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
	}

    startproject "Jario"
	flags
	{
		"MultiProcessorCompile"
	}
	platforms
	{
		"x64", "x86"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "2D"
	location "2D"
	kind "StaticLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"$(SolutionDir)3rdparty/imgui-sfml/imgui-SFML.cpp",
		"$(SolutionDir)3rdparty/imgui/imgui.h",
	}
	includedirs
	{
		"$(SolutionDir)%{prj.name}/src",
		"$(SolutionDir)3rdparty/imgui",
		"$(SolutionDir)3rdparty/imgui-sfml",

	}
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
	filter "configurations:Release"
        defines "RELEASE"
		optimize "On"
	postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/LevelEditor"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Jario"),
		}

project "LevelEditor"
	location "LevelEditor"
	kind "StaticLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		--imgui
		"$(SolutionDir)3rdparty/imgui/imgui_tables.cpp",
		"$(SolutionDir)3rdparty/imgui/imconfig.h",
		"$(SolutionDir)3rdparty/imgui/imgui.h",
		"$(SolutionDir)3rdparty/imgui/imgui_internal.h",
		"$(SolutionDir)3rdparty/imgui/imstb_rectpack.h",
		"$(SolutionDir)3rdparty/imgui/imstb_textedit.h",
		"$(SolutionDir)3rdparty/imgui/imstb_truetype.h",

		"$(SolutionDir)3rdparty/imgui/imconfig.h",
		"$(SolutionDir)3rdparty/imgui/imgui_draw.cpp",
		"$(SolutionDir)3rdparty/imgui/imgui.cpp",
		"$(SolutionDir)3rdparty/imgui/imgui_widgets.cpp",
		"$(SolutionDir)3rdparty/imgui/imgui_demo.cpp",
		"$(SolutionDir)3rdparty/imgui/misc/cpp/imgui_stdlib.cpp",

		-- sfml-imgui
		"$(SolutionDir)3rdparty/imgui-sfml/imgui-SFML_export.h",
		"$(SolutionDir)3rdparty/imgui-sfml/imgui-SFML.cpp",
		"$(SolutionDir)3rdparty/imgui-sfml/imgui-SFML.h",
		
	}
	includedirs{
		"$(SolutionDir)3rdparty/imgui",
		"$(SolutionDir)3rdparty/imgui/misc/cpp/",
		"$(SolutionDir)3rdparty/imgui-sfml",
		"$(SolutionDir)2D/src",
	}
	links{
		"2D"
	}
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
        defines "DEBUG"
		symbols "On"
	filter "configurations:Release"
        defines "RELEASE"
		optimize "On"
    
	postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Jario"),
    }

project "Jario"
    location "Jario"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

    }
    includedirs{
        "$(SolutionDir)2D/src",
		"$(SolutionDir)LevelEditor/src",
		"$(SolutionDir)3rdparty/imgui/misc/cpp/",
		"$(SolutionDir)3rdparty/imgui",
		"$(SolutionDir)3rdparty/imgui-sfml",
    }
    links{
        "2D",
        "LevelEditor",
    }
    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"
    
		