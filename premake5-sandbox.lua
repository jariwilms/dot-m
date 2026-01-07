workspace "dot-m"
	startproject   "sandbox"
	configurations { "Debug", "Release", }
	architecture   "x86_64"

	filter "configurations:Debug"
		runtime   "Debug"
		symbols   "On"
		targetdir "%{wks.location}/bin/debug/%{prj.name}"
		objdir    "%{wks.location}/build/debug/%{prj.name}"

	filter "configurations:Release"
		runtime   "Release"
		optimize  "Full"
		targetdir "%{wks.location}/bin/release/%{prj.name}"
		objdir    "%{wks.location}/build/release/%{prj.name}"
	
	include "premake5.lua"
		
	project "sandbox"
		location         "sandbox"
		language         "C++"
		cppdialect       "C++23"
		kind             "ConsoleApp"
		staticruntime    "On"
		enablemodules    "On"
		buildstlmodules  "On"
		warnings         "Extra"
		externalwarnings "Off"
		
		includedirs { 
			"dot-m/source", 
			"sandbox", 
		}
		files { 
			"sandbox/**.hpp", 
			"sandbox/**.cpp", 
		}
		links { 
			"dot-m", 
		}
