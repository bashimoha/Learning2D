workspace "Learning2D"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
	}

    startproject "2DGame"
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
		--box 2d
		"$(SolutionDir)3rdparty/box2d/include/box2d/**.h",
		--collision
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_broad_phase.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_chain_shape.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_circle_shape.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_collide_circle.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_collide_polygon.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_collide_edge.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_collision.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_distance.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_dynamic_tree.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_edge_shape.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_polygon_shape.cpp",
		"$(SolutionDir)3rdparty/box2d/src/collision/b2_time_of_impact.cpp",
		--common
		"$(SolutionDir)3rdparty/box2d/src/common/b2_block_allocator.cpp",
		"$(SolutionDir)3rdparty/box2d/src/common/b2_draw.cpp",
		"$(SolutionDir)3rdparty/box2d/src/common/b2_math.cpp",
		"$(SolutionDir)3rdparty/box2d/src/common/b2_settings.cpp",
		"$(SolutionDir)3rdparty/box2d/src/common/b2_stack_allocator.cpp",
		"$(SolutionDir)3rdparty/box2d/src/common/b2_timer.cpp",
		--dynamics
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_body.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_chain_circle_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_chain_circle_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_chain_polygon_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_chain_polygon_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_circle_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_circle_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_contact_manager.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_contact_solver.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_contact_solver.h",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_distance_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_edge_circle_contact.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_edge_circle_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_edge_polygon_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_edge_polygon_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_fixture.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_friction_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_gear_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_island.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_island.h",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_motor_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_mouse_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_polygon_circle_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_polygon_circle_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_polygon_contact.cpp ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_polygon_contact.h ",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_prismatic_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_pulley_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_revolute_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_weld_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_wheel_joint.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_world.cpp",
		"$(SolutionDir)3rdparty/box2d/src/dynamics/b2_world_callbacks.cpp",
		--rope
		"$(SolutionDir)3rdparty/box2d/src/rope/b2_rope.cpp",
	}

	includedirs
	{
		"$(SolutionDir)%{prj.name}/src",
		"$(SolutionDir)3rdparty/imgui",
		"$(SolutionDir)3rdparty/imgui-sfml",
		"$(SolutionDir)3rdparty/box2d/src/",
		"$(SolutionDir)3rdparty/box2d/include/",
		"$(SolutionDir)3rdparty/box2d/include/box2d",
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
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/2DGame"),
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
		"$(SolutionDir)3rdparty/box2d/include/box2d/ ",
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
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/2DGame"),
    }

project "2DGame"
    location "2DGame"
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
		"$(SolutionDir)3rdparty/box2d/include/box2d/ ",

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
-- import premake5.lua