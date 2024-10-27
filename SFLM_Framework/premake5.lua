workspace "SFLM_Framework"
	configurations { "Debug", "Release" }
	platforms { "Win32", "x64" }
	startproject "SFLM_Framework"

outputdir = "%{wks.location}/../Bin/"
objoutdir = "%{wks.location}/../Bin-Int/"
dependdir = "%{wks.location}/../Dependencies/"

project "SFLM_Framework"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	files { "**.h", "**.inl", "**.cpp" }

	includedirs { "%{wks.location}", "include", dependdir .. "SFML/include" }
	libdirs { dependdir .. "SFML/lib/%{cfg.platform}" }

	links { "flac", "freetype", "ogg", "openal32", "sfml-audio", "sfml-graphics", "sfml-main", "sfml-network", "sfml-system", "sfml-window", "vorbis", "vorbisenc", "vorbisfile" }

	targetdir (outputdir .. "%{prj.name}/%{cfg.buildcfg}/%{cfg.platform}")
	objdir (objoutdir .. "%{prj.name}/%{cfg.buildcfg}/%{cfg.platform}")
	characterset ("MBCS")
	debugdir "%{wks.location}../Bin/%{prj.name}/%{cfg.buildcfg}/%{cfg.platform}"

	postbuildcommands {

		"xcopy /e /y /i /r \"%{wks.location}..\\Dependencies\\SFML\\lib\\%{cfg.platform}\\*.lib\" \"%{wks.location}..\\Bin\\%{prj.name}\\%{cfg.buildcfg}\\%{cfg.platform}\\\"",
		"xcopy /e /y /i /r \"%{wks.location}..\\Dependencies\\SFML\\dll\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\Bin\\%{prj.name}\\%{cfg.buildcfg}\\%{cfg.platform}\\\"",

		"EXIT /B 0"
	}

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"